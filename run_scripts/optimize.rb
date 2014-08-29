#------------------------------------------------------------------------------
# Parameter Sweeping and Optimization
#------------------------------------------------------------------------------

require_relative 'runscripts'
require_relative '../parsers/parsers'
require_relative '../parsers/m5out'
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
      runmode: :qsub
   )
   HillClimber.new o.merge(
     nametag: "above",
     :@l2l3req_tl => 12,
     :@l2l3resp_tl => 12,
     :@membusreq_tl => 12,
     :@membusresp_tl => 12,
     :@tl0 => 75,
   )
   HillClimber.new o.merge(
     nametag: "below",
     :@l2l3req_tl => 7,
     :@l2l3resp_tl => 7,
     :@membusreq_tl => 7,
     :@membusresp_tl => 7,
     :@tl0 => 75,
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
        take_step
        wait_until_done
        d = find_direction
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
      [-1].product(@params).each do |step,pi|
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
          findTime("results/stdout_climbing_step_#{@step}_#{ps p}_" +
                   "#{instance_variable_get(p)-1}_#{@evalopts[:nametag]}.out")[1]
        }.reduce(:|)
        iterations += 1
        sleep 60
      end
    end

    def find_direction
      ## Current problem: Globs in working directory, not results
      wd = Dir.pwd
      Dir.chdir @evalopts[:exectime]? "results" : "m5out"
      directions = Dir['*'].select {|f|
        f =~ /\w*climbing_step_#{@step}\w*_#{@evalopts[:nametag]}\w*/
      }
      directions.each do |f|
        datum = @evalopts[:exectime]?
          Parsers::findTime(f)[0].to_i :
          Parsers::get_datum(f,Parsers::MEMLATENCY)[0].to_i
        if datum < @min
          @params.each { |p|
            m = f.match(/\w*#{ps(p)}_(\d*)\w*_#{@evalopts[:nametag]}\w*/)
            instance_variable_set(p,m[1].to_i) unless m.nil?
          }
          @min = datum
          Dir.chdir wd
          return true
        end
      end
      Dir.chdir wd
      return false
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
      } || @tl0 < 44
    end

  end

end

if __FILE__ == $0
  hill_climb

end
