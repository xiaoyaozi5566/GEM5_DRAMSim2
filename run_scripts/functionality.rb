#------------------------------------------------------------------------------
# Security Tests
#------------------------------------------------------------------------------
require_relative 'runscripts'
require_relative 'performance'
require 'colored'
include RunScripts


module RunScripts

    def insecure opts={}
     yield ({ 
      cpus: %w[detailed],
      schemes: %w[none],
      benchmarks: $specint,
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

    def only_mc
      insecure{|opts| parallel_local opts.merge(
        nametag:"onlymc", schemes: %w[tp], addrpar: true)}
    end

    def only_l2l3
      insecure{|opts| parallel_local opts.merge(
        nametag: "onlyl2l3", addrpar: true, rr_l2l3: true)}
    end

    def only_membus
      insecure{|opts| parallel_local opts.merge(
        nametag: "onlymembus", addrpar: true, rr_mem: true,
        #It turns out that the following line is not necessary
        split_rport: true, split_mshr: true
      )}
    end

    def only_waypart
      insecure{|opts| parallel_local opts.merge(
        nametag: "onlywaypart", addrpar: true, waypart: true)}
    end

    def debug_only_waypart
      insecure{ |opts| parallel_local opts.merge(
        benchmarks: %w[astar], otherbench: %w[gobmk],
        maxinsts: 10**7,
        nametag: "only_waypart", addrpar: true, waypart: true) }
    end

    def secure_scaling
      secure{|opts| parallel_local_scaling opts}
    end

    def test_memdebug
      insecure{|opts| parallel_local_scaling opts.merge(
          config: "debug/memdebug.py",
          threads:1,
          schemes: %w[tp],
          fixaddr: true
        )
      }
    end

    def test_busdebug
      insecure{|opts| parallel_local_scaling opts.merge(
          config: "debug/busdebug.py",
          threads:1,
          schemes: %w[none],
          rr_nc: true
        )
      }
    end

    def secure_sanity
      secure{|opts| parallel_local opts}
    end

    def insecure_sanity
      insecure{|opts| parallel_local opts}
    end

    def secure_deep
      secure{|opts| parallel_local opts.merge(
        maxinsts: 10**7, fastforward: 100, benchmarks: %w[ mcf sjeng ]
      )}
    end

    def insecure_deep
      insecure{|opts| parallel_local opts.merge(
        maxinsts: 10**6, fastforward: 100, benchmarks: %w[ mcf sjeng ]
      )}
    end

    def check_fwt
      secure do |opts|
        parallel_local opts.merge( maxinsts: 10**6, fastforward: 100,
                                   benchmarks: %w[ mcf sjeng ],
                                   nocwf: true
                                 )
      end
    end

    def check_coordination
      parallel_local_scaling $secure_opts.merge(
        maxinsts: 10**6, fastforward: 100,
        benchmarks: %w[mcf sjeng], nametag: "coordinated",
        coordination: true)
    end

    def test_double_tc
      parallel_local_scaling $secure_opts.merge(
        threads: 1,
        maxinsts: 10**7,
        fastforward: 0,
        nametag: "double_tc",
        benchmarks: $specint - %w[bzip2],
        numcpus: 3,
        numpids: 2,
        do_cache_trace: true,
        skip2: true,
        skip4: true,
        p0threadID: 0,
        p1threadID: 1,
        p2threadID: 1,
        debug: true
      )
    end

end
