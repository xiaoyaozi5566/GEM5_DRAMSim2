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
            cpus: %w[ detailed ],
            schemes: %w[ none ],
            benchmarks: $specint - %w[bzip2],
            maxinsts: 10**3,
            fastforward: 0,
            debug: true,
        }.merge opts

        parallel_local opts
    end

    def sanity_deep_ opts
        opts = {
            cpus: %w[ detailed ],
            benchmarks: %w[ mcf sjeng ],
            maxinsts: 10**6,
            fastforward: 0,
            runmode: :local,
            debug: true
        }.merge opts

        sanity_( opts )
    end

    def sanity
        sanity_( {} )
    end

    def sanity_deep
        sanity_deep_( {} )
    end

    def secure
        sanity_( {
            schemes: %w[tp],
            setpart: true,
            rr_nc: true,
            addrpar: true,
            split_mshr: true,
            split_rport: true,
        } )
    end

    def secure_deep
        sanity_deep_( {
            schemes: %w[tp],
            setpart: true,
            rr_nc: true,
            addrpar: true,
            split_mshr: true,
            split_rport: true,
        } )
    end


end
