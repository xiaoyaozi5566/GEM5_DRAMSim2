#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
require 'parallel'
include RunScripts

module RunScripts

    def cache_security
        opts = {
            maxinsts: 10**3,
            fastforward: 0,
            savetraces: true,
            schemes: %w[ none tp ]
        }
        parallel_local opts

    end

    def execution_time_compare
        opts = {
            benchmarks: $specint - %w[ bzip2 ],
            schemes: %w[ none ],
            maxinsts: 10**6,
            fastforward: 100,
        }
        parallel_local opts

        opts = {
            benchmarks:$specint - %w[ bzip2 ],
            schemes: %w[ tp ],
            maxinsts: 10**6,
            fastforward: 100,
            l3config: "private",
            setpart: true,
            rr_nc: true,
        }
        parallel_local opts
    end

    def execution_time_compare_qsub
        opts = {
            benchmarks:$specint - %w[ bzip2 ],
            schemes: %w[ none ],
            maxinsts: 10**6,
            fastforward: 100,
        }
        qsub_fast opts

        opts = {
            benchmarks:$specint - %w[ bzip2 ],
            schemes: %w[ tp ],
            maxinsts: 10**6,
            fastforward: 100,
            setpart: true,
            rr_nc: true,
        }
        qsub_fast opts
    end

end
