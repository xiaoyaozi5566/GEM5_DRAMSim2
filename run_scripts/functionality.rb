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
            schemes: %w[ none tp ],
            maxinsts: 10**3,
            fastforward: 100,
        }.merge opts

        parallel_local opts
    end

    def sanity_deep_ opts
        opts = {
            cpus: %w[ detailed ],
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
        sanity_( {setpart: true} )
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
