#------------------------------------------------------------------------------
# Performance Evaluation
#------------------------------------------------------------------------------
require_relative 'runscripts'
include RunScripts

module RunScripts

    $secure_opts = {
      schemes: %w[tp],
      addrpar: true,
      rr_nc: true,
      use_way_part: true,
      split_mshr: true,
      split_rport: true
    }

    def baseline
      qsub_scaling(
        schemes: %w[none],
        cpus: %w[detailed],
        maxinsts: 10**9
      )
    end

    def baseline_nocwf
      qsub_scaling(
        schemes: %w[none],
        cpus: %w[detailed],
        otherbench: %w[astar mcf],
        nocwf: true
      )
    end

    def scalability_qsub
        qsub_scaling $secure_opts.merge(
          maxinsts: 10**9
        )
    end

    def scalability_local
     parallel_local_scaling $secure_opts.merge(
       maxinsts: 10**9
     ) 
    end

    def breakdown

      qsub_fast(
        maxinsts: 10**9,
        nametag: "only_l2l3",
        addrpar: true,
        rr_l2l3: true,
        schemes: %w[none]
      )

      qsub_fast(
        maxinsts: 10**9,
        nametag: "only_membus",
        addrpar: true,
        rr_mem: true,
        schemes: %w[none]
      )

      qsub_fast(
        maxinsts: 10**9,
        nametag: "only_waypart",
        addrpar: true,
        waypart: true,
        schemes: %w[none]
      )

      qsub_fast(
        maxinsts: 10**9,
        nametag: "only_mc",
        addrpar: true,
        schemes: %w[tp]
      )

    end

end
