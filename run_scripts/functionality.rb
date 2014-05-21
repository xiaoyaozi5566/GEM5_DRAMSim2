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
        sanity_( { schemes: %w[tp], setpart: true, rr_nc: true } )
    end

    def secure_deep
        sanity_deep_( { schemes: %w[tp], setpart: true, rr_nc: true } )
    end

    def split_mshr
        sanity_( {split_mshr: true, result_dir:"results_split"} )
    end

    def split_mshr_deep
        sanity_deep_( {split_mshr: true, result_dir:"results_split"} )
    end

    def split_mshr_secure
        sanity_( {
            split_mshr: true,
            schemes: %w[tp],
            setpart: true,
            rr_nc: true,
            result_dir:"results_split"
        } )
    end

    def split_mshr_secure_deep
        sanity_deep_( {
            split_mshr: true,
            schemes: %w[tp],
            setpart: true,
            rr_nc: true,
            result_dir:"results_split"
        } )
    end

    def split_rport
        sanity_({split_rport: true })
    end

    def split_rport_deep
        sanity_deep_({split_rport: true })
    end

    def split_rport_secure
        sanity_( {
            split_rport: true,
            schemes: %w[tp],
            setpart: true,
            rr_nc: true,
        } )
    end

    def split_rport_secure_deep
        sanity_deep_( {
            split_rport: true,
            schemes: %w[tp],
            setpart: true,
            rr_nc: true,
        } )
    end

end
