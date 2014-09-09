Dir["*rb"].each { |f| require_relative f }

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

  #Memoized Constructor
  @@caches = {}
  def self.new_m o
    @@caches[o.merge(time: 0)] ||= self.new(o)
  end

  def cycle_complete(t) @t = t + @t_l3 end
  def update(t) @t = t end
  def self.new_m(o) new(o) end

  def to_s
    "(#{"%4d" % @t}) " + "%10s" % @name +
    (SHIFT ? " " * (1+@t) : " ") +
    WHITESQ * @t_l3
  end
end
