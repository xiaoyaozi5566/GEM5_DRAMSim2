require 'colored'

class NumericBusLayer

  def initialize o = {ntcs: 2, offset: 0, turn_l: 1}
    @ntcs, @offset, @turn_l = o[:ntcs], o[:offset], o[:turn_l]
  end


  def cycle_complete time, message_length
    t = time
    m = message_length

    while m > 0 do
      puts "m = #{m}"
      print_stats t

      #Wait until you can consume
      t = next_avail t

      #Consume what can be sent now
      left = t_left t
      t += [m,left].min
      m -= left

    end

    return t
    
  end
  
  def print_stats t
    [ :avail, :next_avail, :t_left, :curr_turn, :time_o ].each do |p|
      eval "puts \"#{p.to_s} #{t} = #{method(p).call t}\""
    end
  end

  def schedule() @turn_l * @ntcs end
  def avail(t) curr_turn(t) == 0 end
  def next_avail(t) avail(t) ? t : t + schedule - (t % schedule) end
  def t_left(t) avail(t) ? @turn_l - (t % schedule) : 0 end
  def curr_turn(t) time_o(t) / @turn_l % @ntcs  end
  def time_o(t) t - @offset end

end

class BusLayer

  def initialize o = {}
    o = {
      ntcs: 2, offset: 0, turn_l: 1,
      name: "", message: 1
    }.merge o
    @message_length  = o[:message]
    @name = o[:name]
    @time = 0
    @slots = [true] * o[:turn_l] +
      [false] * o[:turn_l] * (o[:ntcs] - 1)
    @slots = @slots.rotate(-o[:offset])
  end

  #Memoized Constructor
  @@buses = {}
  def self.new_m o
    @@buses[o] ||= self.new o 
  end

  def cycle_complete time

    update time

    @message_length.times do
      consume while !@slots[0]
      consume
    end

    @time

  end

  def consume
    @slots.rotate!
    @time +=1
  end

  def update time
    @slots.rotate!(time-@time)
    @time = time
  end

  WHITESQ = "\u25A1"
  BLKSQ   = "\u25A0"
  def to_s
    "(#{"%4d" % @time}) " + "%10s" % @name +
      " #{@slots.inject(""){ |s,slot| s += slot ? BLKSQ : WHITESQ }}"
  end
  
end

class PipelinedCache
  WHITESQ = "\u25A0"
  def initialize o = {}
    o = {
      name: "L3",
      t_l3: 9
    }.merge o
    @name = o[:name]
    @t_l3 = o[:t_l3]
    @t    = 0
  end

  def cycle_complete(t) @t = t + @t_l3 end
  def update(t) @t = t end
  def self.new_m(o) new(o) end

  def to_s
    "(#{"%4d" % @t}) " + "%10s" % @name + " " + WHITESQ * @t_l3
  end
end

def simulate_l3_hit o = {}
  o = {
    # System Configuration
    l2l3req_tl:   1,
    l2l3req_o:    0,
    l2l3resp_tl:  9,
    l2l3resp_o:   0,
    l3memreq_tl:  1,
    l3memreq_o:   0,
    l3memresp_tl: 9,
    l3memresp_o:  0,
    # Environment
    t_l3:  9,
    t_req:  1,
    t_resp: 9,
    ntcs:   2,
    time:   0,
  }.merge o

  [
    BusLayer.new_m(
      name:    "l2l3_req",
      message: o[:t_req],
      ntcs:    o[:ntcs],
      turn_l:  o[:l2l3req_tl],
      offset:  o[:l2l3req_o],
    ),
    PipelinedCache.new_m(
      name: "L3",
      t_l3:  o[:t_l3]
    ),
    BusLayer.new_m(
      name:    "l2l3_resp",
      message: o[:t_resp],
      ntcs:    o[:ntcs],
      turn_l:  o[:l2l3resp_tl],
      offset:  o[:l2l3resp_o],
    )
  ].inject(o[:time]) do |time, component|
    component.update time #happens automatically, needed for print
    puts component.to_s.green if DEBUG
    time = component.cycle_complete time
  end
end

if __FILE__ == $0
  DEBUG = true
  # a = SlotBusLayer.new(ntcs: 4, offset: 0, turn_l: 3, name: "l2l3_req")
  # puts a.cycle_complete 0, 3

  puts simulate_l3_hit.to_s.magenta

end
