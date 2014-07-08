#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative "runscripts"
require 'parallel'
require 'colored'
include RunScripts

module RunScripts
    def insecure opts={}
     yield ({ 
      cpus: %w[detailed],
      schemes: %w[none],
      benchmarks: $specint - %w[bzip2],
      maxinsts: 10**3,
      fastforward: 0,
      debug: true,
      runmode: :local,
     }.merge opts)
    end

    def secure opts={}, &block
      insecure ({
        schemes: %w[tp],
        waypart: true,
        rr_nc: true,
        addrpar: true,
        split_rport: true,
        split_mshr: true
      }.merge opts){|args| block.call args}
    end

    def secure_scaling
      secure{|opts| parallel_local_scaling opts}
    end

    def secure_sanity
      secure{|opts| parallel_local opts}
    end

    def insecure_sanity
      insecure{|opts| parallel_local opts}
    end

    def secure_deep
      secure{|opts| parallel_local opts.merge(
        maxinsts: 10**6, fastforward: 100, benchmarks: %w[ mcf sjeng ]
      )}
    end

    def insecure_deep
      insecure{|opts| parallel_local opts.merge(
        maxinsts: 10**6, fastforward: 100, benchmarks: %w[ mcf sjeng ]
      )}
    end

end
