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
            maxinsts: 10**6,
            fastforward: 100,
        }
        parallel_local opts

        opts = {
            schemes: %w[ tp ],
            maxinsts: 10**6,
            fastforward: 100,
            setpart: true,
            rr_nc: true,
        }
        parallel_local opts
    end

end
