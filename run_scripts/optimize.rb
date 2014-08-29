#------------------------------------------------------------------------------
# Parameter Sweeping and Optimization
#------------------------------------------------------------------------------

require_relative 'runscripts'
require_relative '../parsers/parsers'
include RunScripts

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
      HillClimber.new $secure_opts.merge(
      )
    end

    #Don't do configuration here do it in hill_climb abive.
    class HillClimber

      def initialize(evalopts={})

        @evalopts = evalopts
        @min = Float::INFINITY
        @step = 0

        @params = [
          :@l2l3req_tl,
          :@l2l3resp_tl,
          :@membusreq_tl,
          :@membusresp_tl,
          :@tl0,
        ]

        @params.each { |p| set_instance_variable(p, evalopts[p]) }

        while @step < @max_steps
          take_step
          wait_until_done
          find_direction
          @step += 1
          break if direction_is_crazy
        end
      end

      def ps param_symbol
        param_symbol.to_s.delete(":@")
      end

      def take_step
        [-1].product(@params).each do |step,pi|
          new_p = instance_variable(pi) + step
          break if step_is_unsafe pi, new_p
          iterate_and_submit @params.inject({}){ |hsh,pj| 
            hsh[pj] = instance_variable(pj); hsh
          }.merge(
            pi => new_p,
            :tl1 => pi == :@tl0 ? new_p : @tl0
          ).merge(
            filename: "climbing_step_#{@step}_#{ps(pi)}_#{new_p}"
          )
        end
      end

      def wait_until_done
        # check if jobs are done and data is available
        # should not wait forever if the jobs crash or take too long
      end

      def find_direction
        directions = Dir['*'].select {|f| f =~ /climing_step_#{@step}/}
        directions.each do |f|
          time = findTime(f)[0]
          if time < @min
            @params.each { |p|
              instance_variable_set(p,
                f.match(/#{p.to_s.delete("@")}_(\d*)/)[1])
            }
            @min = time
          end
        end
      end

      def direction_is_crazy
        return false if @mem < 64
        @params.inject(false) do |iscrazy,p|
          iscrazy = iscrazy || instance_variable(p) < 0
        end
      end

      def step_is_unsafe param
        #whether or not param is out of bounds
      end

    end

end
