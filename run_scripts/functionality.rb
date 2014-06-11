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
            split_rport: true,
            split_mshr: true,
        } )
    end
    ###########################################################################
    # Currently fails:
    # tp_detailed_gcctl6_astartl6_c4MB
    ###########################################################################

    def secure_deep
        sanity_deep_({
            schemes: %w[tp],
            setpart: true,
            rr_nc: true,
            addrpar: true,
            split_rport: true,
            split_mshr: true,
        } )
    end

    def secure_wp
        sanity_({
            schemes: %w[tp],
            waypart: true,
            rr_nc: true,
            addrpar: true,
            split_rport: true,
            split_mshr: true,
        } )
    end

    def secure_wp_deep
        sanity_deep_({
            schemes: %w[tp],
            aypart: true,
            rr_nc: true,
            addrpar: true,
            split_rport: true,
            split_mshr: true,
        } )
    end

    def gcc_l2l3bus_hang
        opts = {
            nametag: "uncoordinated",
            p1: "astar",
            rr_nc: true,
            addrpar: true,
            setpart: true,
            split_rport: true,
            split_mshr: true,
            runmode: :local,
        }

        #Coordinated Scheme
        opts_coord = opts.merge({
            nametag: "coordinated",
            l2l3req_tl:        12,
            l2l3req_offset:     0,
            l2l3resp_tl:       12,
            l2l3resp_offset:   12,
            membusreq_tl:       1,
            membusreq_offset:   0,
            membusresp_tl:     96,
            membusresp_offset: 49,
            dramoffset:      -120,
        })
        Parallel.each(1..2, :in_threads=>2) do |i|
            if i==1
                sav_script( "detailed", "tp", "gcc", opts )
            else
                sav_script( "detailed", "tp", "gcc", opts_coord )
            end
        end
    end

end
