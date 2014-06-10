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
            schemes: %w[ none ],
            benchmarks: $specint - %w[ bzip2 ],
            cpus: %w[detailed],
            maxinsts: 10**5,
            fastforward: 0,
        }
        #parallel_local opts

        opts = opts.merge({
            schemes: %w[ tp ],
            setpart: true,
            addrpar: true,
            split_mshr: true,
            rr_nc: true,
            debug: true,
        })
        parallel_local opts
    end

    def setpart_vs_private
        opts = {
            l3config: "private",
            benchmarks:$specint - %w[ bzip2 ],
            schemes: %w[ tp ],
            maxinsts: 10**6,
            fastforward: 0,
            addrpar: true,
            rr_nc: true,
            cpus: %w[detailed],
            result_dir: "results_private"
        }
        parallel_local opts

        opts = opts.merge({
            l3config: "shared",
            split_mshr: true,
            setpart: true,
            result_dir: "results_setpart"
        })

        parallel_local opts
    end

    def execution_time_compare_qsub
        opts = {
            benchmarks:$specint - %w[ bzip2 ],
            schemes: %w[ none ],
            cpus: %w[detailed],
        }
        qsub_fast opts

        opts = opts.merge({
            schemes: %w[ tp ],
            setpart: true,
            addrpar: true,
            split_mshr: true,
            split_rport: true,
            rr_nc: true,
        })
        qsub_fast opts
    end

    def security_debug
        opts = {
            p1: "astar",
            maxinsts: 10**6,
            fastforward: 0,
            setpart: true,
            rr_nc: true,
            split_mshr: true,
            memdebug: true,
            runmode: :local
        }
        Parallel.each(1..2, :in_threads=>2) do |i|
            if i==1
                sav_script( "detailed", "tp", "mcf", opts )
            else
                opts = opts.merge( { p1: "hmmer" } )
                sav_script( "detailed", "tp", "mcf", opts )
            end
        end
    end

end
