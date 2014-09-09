Dir['*rb'].each { |f| require_relative f }

class Integer
  def to_bus() (self*1.5).ceil end
end

class MemoryController
  def initialize o={}
    o = {
      name:        "MemCtl",
      ntcs:        2,
      offset:      0,
      t_mem:       44.to_bus, # total turn length, not when data is available
      wc:          43.to_bus,
      wc_read:     24.to_bus # tRCD + tCAS + WL/2
    }.merge o

    @name = o[:name]
    @time = 0
    @message = o[:wc_read]

    # Model assumes that a request can only be started in the right turn,
    # and not during the dead time. If a request cannot finish by the dead 
    # time, it waits until the next turn
   
    dead_time = (o[:t_mem] - (o[:t_mem] - o[:wc])/15.0).ceil
    dead_time = o[:wc] if o[:t_mem] == o[:wc]
    @slots = [:active] * (o[:t_mem] - dead_time) +
      [:dead] * dead_time +
      [:other] * o[:t_mem] * (o[:ntcs] - 1)
    @slots.rotate!(-o[:offset])
  end

  @@memcs = {}
  def self.new_m o
    @@memcs[o.merge(time: 0)] ||= self.new o
  end

  def cycle_complete t
    # Advance to cycle t
    update t

    # Advance to the next @message consecutive cycles of a turn that begins 
    # outside of the dead time. 
    update( @time + 1) until @slots[0] == :active && @slots[@message] != :other

    #Consume the entire memory access
    update( @time + @message )

  end

  def update time
    @slots.rotate!(time-@time)
    @time = time
  end

  WHITESQ = "\u25A1"
  BLKSQ   = "\u25A0"
  BLKDIA  = "\u25C6"

  def sym_to_s sym
    ( sym == :active && BLKSQ ) ||
      ( sym == :dead && BLKDIA )  ||
      ( sym == :other && WHITESQ )
  end

  def to_s
    "(#{"%4d" % @time}) " + "%10s" % @name +
      (SHIFT ? " " *  @time : "") +
      " #{@slots.inject(""){ |s,slot| s += sym_to_s slot }}"
  end

end

class NonPreemptingMC < MemoryController

  def initialize o={}
    super o
    dead_time = o[:wc]
    @slots = [:active] * (o[:t_mem] - dead_time) +
      [:dead] * dead_time +
      [:other] * o[:t_mem] * (o[:ntcs] - 1)
    @slots.rotate!(-o[:offset])
  end

end
