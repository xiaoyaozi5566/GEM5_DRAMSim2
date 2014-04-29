#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
require 'parallel'
include RunScripts

module RunScripts

    def test_sanity

        cpus = $cpus # %w[ timing ]
        schemes = $schemes #w%w[ tp ]
        benchmarks = $specint #%w[ mcf sjeng ]
        opts = {
            maxinsts: 10**3,
            fastforward: 100,
            runmode: :local,
        }

        cpus.product( schemes ).each do |cpu, scheme|
            Parallel.each( benchmarks.combination( 2 ),
                          in_threads: 4 ) do |p0, p1|
                iteropts = { p1: p1 }
                sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
            end
        end

    end

end
