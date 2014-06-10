#------------------------------------------------------------------------------
# Performance Evaluation
#------------------------------------------------------------------------------
require_relative 'runscripts'
include RunScripts

module RunScripts
    def preliminary_results
        # Baseline
        opts = {
            cpus: %w[ detailed ],
            schemes: %w[ none ],
            benchmarks: $specint - %w[ bzip2 ],
            result_dir: "results_preliminary"
        }
        yield opts

        #Naive Secure Scheme
        opts = opts.merge({
            schemes: %w[ tp ],
            otherbench: %w[ astar ],
            rr_nc: true,
            addrpar: true,
            setpart: true,
            split_rport: true,
            split_mshr: true,
        })
        yield opts
    end

    def prelim_qsub
        preliminary_results{|opts| qsub_fast opts}
    end

    def prelim_local
        preliminary_results{|opts|
            parallel_local opts.merge(maxinsts: 10**5, fastforward: 10)
        }
    end

    def scalability_qsub
        preliminary_results{|opts|
            qsub_fast_scaling opts.merge(
                result_dir: "results_scalability"
            )
        }
    end

    def scalability_local
        preliminary_results{|opts|
            parallel_local_scaling opts.merge(
                maxinsts: 10**5,
                fastforward: 10,
                result_dir: "results_scalability"
            )
        }
    end

    def coordination
        # Naive Secure Scheme
        opts = {
            cpus: %w[ detailed ],
            benchmarks: $specint - %w[ bzip2 ],
            otherbench: %w[ astar ],
            result_dir: "results_coordination",
            schemes: %w[ tp ],
            rr_nc: true,
            addrpar: true,
            setpart: true,
            split_rport: true,
            split_mshr: true,
        }
        yield opts

        # Better Baseline
         opts = opts.merge({
             nametag: "better_base",
             l2l3req_tl:        1,
             l2l3req_offset:    0,
             l2l3resp_tl:      10,
             l2l3resp_offset:   0,
             membusreq_tl:      1,
             membusreq_offset:  0,
             membusresp_tl:    10,
             membusresp_offset: 0,
             dramoffset:        0
         })
        # Better baseline is deprecated because it isn't better.
        # yield opts

        #Coordinated Scheme
        opts = opts.merge({
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
        yield opts
    end

    def coordination_qsub
        coordination{|opts|
            qsub_fast opts
        }
    end

    def coordination_local
        coordination{|opts|
           parallel_local opts.merge( maxinsts: 10**5, fastforward: 10 )
        }
    end

    def breakdown
        # Baseline
        opts = {
            cpus: %w[ detailed ],
            schemes: %w[ none ],
            benchmarks: $specint - %w[ bzip2 ],
            result_dir: "results_breakdown"
        }
        yield opts

        opts = opts.merge({
            addrpar: true,
        })

        # RR Bus
        yield opts.merge({ nametag: "rrbus", rr_nc: true })

        # Set Partitioining
        yield opts.merge({ nametag: "setpart", setpart: true })

        # Split MSHR Queues
        yield opts.merge({ nametag: "splitmshr", split_mshr: true })

        # Split Response Ports 
        yield opts.merge({ nametag: "splitrport", split_rport: true })

        # TP memory controller
        yield opts.merge({ schemes: %w[ tp ] })

    end

    def breakdown_qsub
        breakdown{|opts| qsub_fast opts}
    end

    def breakdown_local
        breakdown{|opts|
            parallel_local opts.merge(maxinsts: 10**5, fastforward: 10)
        }
    end

    def multicore_tc_qsub
        puts "multicore_tc_qsub is unimplemented"
    end

end
