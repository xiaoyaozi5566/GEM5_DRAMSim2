#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
include RunScripts

module RunScripts

    def cache_security

        cpus = %w[ timing ]
        schemes = %w[ tp ]
        l3configs = %w[ shared private ]
        benchmarks = %w[ astar bzip2 ]
        opts = {
            maxinsts: 10**5,
            fastforward: 0,
            runmode: :local
        }

        cpus.product( schemes, l3configs ).each do |cpu, scheme, l3config|
            benchmarks.combination( 2 ).each do |p0, p1|
                iteropts = { p1: p1, l3config: l3config }
                sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
            end
        end

    end

end
