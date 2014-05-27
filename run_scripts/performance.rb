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
            fastforward: 0,
            maxinsts: 10**8,
            schemes: %w[ none ],
            benchmarks: $specint - %w[ bzip2 ],
            result_dir: "results_preliminary"
        }
        yield opts

        #Naive Secure Scheme
        opts = opts.merge({
            schemes: %w[ tp ],
            rr_nc: true,
            addprar: true,
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
        preliminary_results{|opts| parallel_local opts}
    end

    def scalability
        # Baseline
        opts = {
            cpus: %w[ detailed ],
            fastforward: 10**9,
            maxinsts: 10**8,
            schemes: %w[ none ],
            benchmarks: $specint - %w[ bzip2 ],
            result_dir: "results_scalability"
        }
        yield opts

        #Naive Secure Scheme
        opts = opts.merge({
            schemes: %w[ tp ],
            rr_nc: true,
            addprar: true,
            setpart: true,
            split_rport: true,
            split_mshr: true,
        })
        yield opts
    end

    def scalability_qsub
        perf_breakdown{|opts| qsub_fast opts}
    end

    def perf_breakdown
        # Baseline
        opts = {
            cpus: %w[ detailed ],
            fastforward: 10**9,
            maxinsts: 10**8,
            schemes: %w[ none ],
            benchmarks: $specint - %w[ bzip2 ],
            result_dir: "results_perf_breakdown"
        }
        yield opts

        opts = opts.merge({ addrpar: true })

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

    def perf_breakdown_qsub
        perf_breakdown{|opts| qsub_fast opts}
    end

    def multicore_tc_qsub
        puts "multicore_tc_qsub is unimplemented"
    end

end
