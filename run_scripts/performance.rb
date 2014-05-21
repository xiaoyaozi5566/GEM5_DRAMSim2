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
            benchmarks: $specint, #- %w[ bzip2 ],
            result_dir: "results_preliminary"
        }
        yield opts

        #Naieve Secure Scheme
        opts = opts.merge({
            schemes: %w[ tp ],
            rr_nc: true,
            addprar: true,
            setpart: true,
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

end
