require_relative 'runscripts'
include RunScripts

module RunScripts
  def l2l3_sweeping opts={}
    opts = { schemes: %w[none], otherbench: %w[astar mcf] }.merge opts

    #sweep resp turn
    [1,4,8,9,12,17,18].each do |tl|
      #default req turn
      o = opts.merge(
        nametag: "l2l3resp_#{tl}",
        rr_l2l3: true,
        l2l3resp_tl: tl
      )
      yield o
    end
  end

  def l2l3_sweeping_l3offset opts = {}
    opts = { schemes: %w[none], otherbench: %w[astar mcf] }.merge opts

    # sweep resp turn, match req turn
    [1,4,8,9,12,17,18].each do |tl|
      #default req turn
      o = opts.merge(
        nametag: "l2l3resp_#{tl}",
        rr_l2l3: true,
        l2l3resp_tl: tl,
        l2l3req_tl:  tl,
        l2l3req_offset: 9
      )
      yield o
    end
  end

  def l2l3_sweeping_local
    opts = { maxinsts: 10**3, fastforward: 100, nocwf: true }
    l2l3_sweeping(opts) do |o|
      parallel_local_scaling o
    end
  end

  def l2l3_sweeping_qsub
    l2l3_sweeping { |o| qsub_scaling o.merge(nocwf: true) }
  end

  def l2l3_sweeping_l3offset_local
    opts = { maxinsts: 10**3, fastforward: 100, nocwf: true }
    l2l3_sweeping_l3offset(opts) do |o|
      parallel_local_scaling o
    end
  end

  def l2l3_sweeping_l3offset_qsub
    l2l3_sweeping_l3offset { |o| qsub_scaling o.merge(nocwf: true) }
  end

  def membus_sweeping opts={}
    opts = { schemes: %w[none], otherbench: %w[astar mcf],
    }.merge opts

    #sweep resp turn
    [1,4,8,9,12,17,18].each do |tl|
      #default req turn
      o = opts.merge(
        nametag: "membusresp_#{tl}",
        rr_mem: true,
        membusresp_tl: tl
      )
      yield o
    end
  end

  def membus_sweeping_local
    opts = { maxinsts: 10**3, fastforward: 100 }
    membus_sweeping(opts) do |o|
      parallel_local_scaling o
    end
  end

  def membus_sweeping_qsub
    membus_sweeping { |o| qsub_scaling o.merge(nocwf: true) }
  end

  def memctl_sweeping opts={}
    opts = {
      schemes: %w[tp],
      otherbench: %w[astar],
      diffperiod: true,
    }.merge opts

    [0,64,96,128,192,256].each do |tl|
      o = opts.merge(
        tl0: tl,
        tl1: tl,
        tl2: tl,
        tl3: tl
      )
      yield o
    end
  end

  def memctl_sweeping_local
    membus_sweeping(maxinsts: 10**3, fastforward: 100, debug: true) do |o|
      parallel_local_scaling o
    end
  end

  def memctl_sweeping_qsub
    memctl_sweeping { |o| qsub_scaling o }
  end

  def fa_sweeping_local
    membus_sweeping( maxinsts: 10**3, fastforward: 100, debug: true ) do |o|
      parallel_local_scaling o.merge(schemes: %w[fa])
    end
  end

  def fa_sweeping_qsub
    memctl_sweeping { |o| qsub_scaling o.merge(schemes: %w[fa]) }
  end
end
