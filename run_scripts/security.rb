#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
require 'parallel'
include RunScripts

module RunScripts

    def cache_security

        cpus = $cpus # %w[ timing ]
        schemes = $schemes #w%w[ tp ]
        l3configs = %w[ shared private ]
        benchmarks = $specint #%w[ mcf sjeng ]
        opts = {
            maxinsts: 10**3,
            fastforward: 0,
            runmode: :local,
            savetraces: true
        }

        cpus.product( schemes, l3configs ).each do |cpu, scheme, l3config|
            Parallel.each( benchmarks.combination( 2 ),
                          in_processes: 4 ) do |p0, p1|
                iteropts = { p1: p1, l3config: l3config }
                sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
            end
        end

    end

end
