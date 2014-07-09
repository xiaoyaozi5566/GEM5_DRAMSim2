require_relative 'runscripts'
include RunScripts

module RunScripts
  def l2l3_sweeping opts={}
    opts = { schemes: %w[tp], otherbench: %w[astar] }.merge opts

    #sweep resp turn
    [1,4,8,9,16,17,25,32].each do |tl|
      #default req turn
      o = opts.merge(
        nametag: "l2l3resp_#{tl}",
        l2l3resp_tl: tl
      )
      yield o
    end
  end

  def l2l3_sweeping_local
    l2l3_sweeping( maxinsts: 10**3, fastforward: 100, debug: true ) do |o|
      parallel_local_scaling o
    end
  end

  def l2l3_sweeping_qsub
    l2l3_sweeping { |o| qsub_scaling o }
  end

  def membus_sweeping opts={}
    opts = { schemes: %w[tp], otherbench: %w[astar] }.merge opts

    #sweep resp turn
    [1,4,8,9,16,17,25,32].each do |tl|
      #default req turn
      o = opts.merge(
        nametag: "membusresp_#{tl}",
        membusresp_tl: tl
      )
      yield o
    end
  end

  def membus_sweeping_local
    membus_sweeping( maxinsts: 10**3, fastforward: 100, debug: true ) do |o|
      parallel_local_scaling o
    end
  end

  def membus_sweeping_qsub
    membus_sweeping { |o| qsub_scaling o }
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
    membus_sweeping( maxinsts: 10**3, fastforward: 100, debug: true ) do |o|
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
