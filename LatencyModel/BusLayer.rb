Dir["*rb"].each { |f| require_relative f }

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
    @@buses[o.merge(time: 0)] ||= self.new o
  end

  # Calculate when the data for a packet entering at "time" will be available
  # based on the parameters from initializaiton
  def cycle_complete time
    update time

    @message_length.times do
      update(@time + 1) while !@slots[0]
      update(@time + 1)
    end

    @time
  end

  def update time
    @slots.rotate!(time-@time)
    @time = time
  end

  WHITESQ = "\u25A1"
  BLKSQ   = "\u25A0"
  def to_s
    "(#{"%4d" % @time}) " + "%10s" % @name +
      (SHIFT ? " " *  @time : "") +
      " #{@slots.inject(""){ |s,slot| s += slot ? BLKSQ : WHITESQ }}"
  end
  
end
