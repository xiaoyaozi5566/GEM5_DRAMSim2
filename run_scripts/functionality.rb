#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
require 'parallel'
require 'colored'
include RunScripts

module RunScripts
    def sanity_ opts
        opts = {
            cpus: $cpus,
            schemes: %w[ none tp ],
            benchmarks: $specint,
            maxinsts: 10**3,
            fastforward: 100,
            runmode: :local,
            debug: true,
        }.merge opts

        failed = []

        opts[:cpus].product( opts[:schemes] ).each do |cpu, scheme|
            Parallel.each( opts[:benchmarks].combination( 2 ),
                          in_threads: 4 ) do |p0, p1|
                iteropts = { p1: p1 }
                r = sav_script( cpu, scheme, p0, opts.merge( iteropts ) )
                failed << r[1] unless r[0] 
            end
        end
        failed.each{|f| puts f.red}
    end

    def sanity_deep_ opts

        opts = {
            cpus: %w[ detailed ],
            schemes: %w[ none tp fa ],
            benchmarks: %w[ mcf sjeng ],
            maxinsts: 10**6,
            fastforward: 100,
            runmode: :local,
            debug: true,
        }.merge opts

        sanity_( opts )

    end

    def sanity
        sanity_( {} )
    end

    def sanity_deep
        sanity_deep_( {} )
    end

    def set_part
        sanity_( {setpart: true } )
    end

    def set_part_deep
        sanity_deep_( {setpart: true} )
    end

    def rr_nc
        sanity_( {rr_nc: true} )
    end

    def rr_nc_deep
        sanity_deep_( {rr_nc: true} )
    end

    def secure
        sanity_( { setpart: true, rr_nc: true } )
    end

    def secure_deep
        sanity_deep_( { setpart: true, rr_nc: true } )
    end

end
