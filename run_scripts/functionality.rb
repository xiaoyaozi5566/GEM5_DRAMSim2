#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
require 'parallel'
include RunScripts

module RunScripts

    def test_sanity

        cpus = $cpus
        schemes = %w[ none tp ]
        benchmarks = $specint
        opts = {
            maxinsts: 10**3,
            fastforward: 100,
            runmode: :local,
            debug: true,
        }

        cpus.product( schemes ).each do |cpu, scheme|
            Parallel.each( benchmarks.combination( 2 ),
                          in_threads: 4 ) do |p0, p1|
                iteropts = { p1: p1 }
                sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
            end
        end

    end

    def test_sanity_deep

        cpus = %w[ detailed ]
        schemes = %w[ none tp ]
        benchmarks = %w[ mcf sjeng ]
        opts = {
            maxinsts: 10**6,
            fastforward: 100,
            runmode: :local,
            debug: true,
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
