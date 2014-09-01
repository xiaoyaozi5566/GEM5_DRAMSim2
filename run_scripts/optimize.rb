#------------------------------------------------------------------------------
# Parameter Sweeping and Optimization
#------------------------------------------------------------------------------

require_relative 'runscripts'
require_relative '../parsers/parsers'
require_relative '../parsers/m5out'
require 'parallel_each'
include RunScripts
include Parsers

module RunScripts

  $secure_opts = {
    schemes: %w[tp],
    addrpar: true,
    rr_nc: true,
    use_way_part: true,
    split_mshr: true,
    split_rport: true
  }

  def hill_climb
   o=$secure_opts.merge(
      maxinsts: 10**8,
      fastforward: 10**9,
      maxiter: 3*60,
      maxsteps: 85,
      exectime: false,
      benchmarks: %w[mcf],
      runmode: :local,
      threads: 1 #multithreading is done in HillClimber
   )
   HillClimber.new o.merge(
     nametag: "above",
     :@l2l3req_tl => 12,
     :@l2l3resp_tl => 12,
     :@membusreq_tl => 12,
     :@membusresp_tl => 12,
     :@tl0 => 70,
   )
   HillClimber.new o.merge(
     nametag: "below",
     :@l2l3req_tl => 7,
     :@l2l3resp_tl => 7,
     :@membusreq_tl => 7,
     :@membusresp_tl => 7,
     :@tl0 => 70,
   )
   HillClimber.new o.merge(
     nametag: "bottom",
     :@l2l3req_tl => 1,
     :@l2l3resp_tl => 1,
     :@membusreq_tl => 1,
     :@membusresp_tl => 1,
     :@tl0 => 43,
   )
  end

  #Don't do configuration here do it in hill_climb above.
  class HillClimber

    def initialize(evalopts={})

      @evalopts = evalopts.merge( diffperiod: true )
      @min = Float::INFINITY
      @step = 0

      @max_steps = evalopts[:maxsteps]

      @outfile = File.open("hill_climb_#{evalopts[:nametag]}",'w')

      @params = [
        :@l2l3req_tl,
        :@l2l3resp_tl,
        :@membusreq_tl,
        :@membusresp_tl,
        :@tl0,
      ]

      @params.each { |p| instance_variable_set(p, evalopts[p]) }

      loop do 
        puts "#{'='*80}\n#{' '*36}step #{@step}#{' '*36}\n#{'='*80}"
        take_step
        puts "took step".green
        # wait_until_done
        # puts "done waiting".green
        d = find_direction
        puts "looked for direction".green
        if !d || direction_is_crazy || (@step > @max_steps)
          print_results
          @outfile.puts "crazy direction" if direction_is_crazy
          break
        end
        print_results
        @step += 1
      end

      @outfile.close

    end

    def print_results
      @outfile.puts @params.map { |p|
        "#{p}, #{instance_variable_get(p)}".green
      } << ["#{@step}".green]
    end

    def ps param_symbol
      param_symbol.to_s.delete("@").intern
    end

    def take_step
      [-2,2].product(@params).p_each(10) do |step,pi|
        new_p = instance_variable_get(pi) + step
        next if step_is_unsafe pi, new_p
        o = @params.inject({}){ |hsh,pj| 
          hsh[ps(pj)] = instance_variable_get(pj); hsh
        }.merge(
          ps(pi) => new_p,
          :tl1 => pi == :@tl0 ? new_p : @tl0
        ).merge(
          filename: "climbing_step_#{@step}_#{ps(pi)}_#{new_p}" +
            "_#{@evalopts[:nametag]}"
        ).merge @evalopts
        iterate_and_submit o
      end
    end

    def wait_until_done
      # check if jobs are done and data is available
      # should not wait forever if the jobs crash or take too long
      iterations = 0
      loop do
        return if iterations > @evalopts[:maxiter]
        return if @params.map{ |p|
          f = "results/stdout_climbing_step_#{@step}_#{ps p}_" +
                "#{instance_variable_get(p)-1}_#{@evalopts[:nametag]}.out"
          puts f
          findTime(f)[1]
        }.reduce(:|)
        puts "1".blue
        iterations += 1
        puts "2".blue
        sleep(3.0)
        puts "3".blue
      end
    end

    def find_direction
      ## Current problem: Globs in working directory, not results
      wd = Dir.pwd
      Dir.chdir @evalopts[:exectime]? "results" : "m5out"
      directions = Dir['*'].select {|f|
        f =~ /\w*climbing_step_#{@step}\w*_#{@evalopts[:nametag]}\w*/
      }

      puts "directions"
      puts directions

      extract = lambda do |f|
        datum = @evalopts[:exectime]?
          Parsers::findTime(f) :
          Parsers::get_datum(f,Parsers::MEMLATENCY)
        puts "#{f} had a bad datum".red unless datum[1]
        datum = datum[1]? datum[0] : Float::INFINITY
      end

      data = directions.inject({}) do |d,f|
        m = f.match(/\w*step_#{@step}_(\w*)\w*_#{@evalopts[:nametag]}\w*/)
        name = m.nil? ? "notfound" : m[1]
        d[name] = extract.call f
        d
      end

      puts "found_data"
      puts data
      puts "current params"
      puts @params.inject(''){ |s,p| s += "(#{p} = #{instance_variable_get(p)}) " }

      next_direction = @min, nil, nil
      directions.each do |f|
        datum = extract.call f
        # datum = @evalopts[:exectime]?
        #   Parsers::findTime(f)[0].to_i :
        #   Parsers::get_datum(f,Parsers::MEMLATENCY)[0].to_i
        param_n, param_v = @params.inject(nil) { |r,p|
            m = f.match(/\w*#{ps(p)}_(\d*)\w*_#{@evalopts[:nametag]}\w*/)
            m.nil? ? [nil, nil] : [p, m[1].to_i]
        }
        if datum < next_direction[0]
          puts "#{datum} < #{next_direction[0]}"
          next_direction = datum, param_n, param_v
        end
      end
      Dir.chdir wd
      if next_direction[1].nil?
        puts "didn't find a direction"
        return false
      else
        @min = next_direction[0]
        instance_variable_set(next_direction[1],next_direction[2])
        return true
      end
    end

    def direction_is_crazy
      return false if @tl0 < 64
      @params.inject(false) do |iscrazy,p|
        iscrazy = iscrazy || instance_variable_get(p) < 0
     end
    end

    def step_is_unsafe param, val
      return @params.inject(false){ |s,p|
        s |= instance_variable_get(p) < 1
      } || @tl0 < 43
    end

  end

end

if __FILE__ == $0
  hill_climb

end
