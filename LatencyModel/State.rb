Dir['*rb'].each { |f| require_relative f }

class State

  def initialize o={}

    @o = {
      max: {
        :l2l3req_tl   => 15,
        :l2l3req_o    => ( o[:l2l3req_tl] || 15) *2, 
        :l2l3resp_tl  => 15,
        :l2l3resp_o   => ( o[:l2l3resp_tl] || 15) *2,
        :l3memreq_tl  => 15,
        :l3memreq_o   => ( o[:l3memreq_tl] || 15) *2,
        :l3memresp_tl => @mem_tl || 80,
        :l3memresp_o  => ( o[:l3memresp_tl] || 80) *2,
        :mem_tl       => 80,
        :mem_o        => ( o[:mem_tl] || 80) *2
      },
      min: {
        :l2l3req_tl   => 1,
        :l2l3req_o    => 0,
        :l2l3resp_tl  => 1,
        :l2l3resp_o   => 0,
        :l3memreq_tl  => 1,
        :l3memreq_o   => 0,
        :l3memresp_tl => 1,
        :l3memresp_o  => 0,
        :mem_tl       => 44,
        :mem_o        => 0
      },
      deltas: lambda{ |p| -3.upto(3).to_a - [0] },
    }.merge o

    @max = @o[:max]
    @min = @o[:min]
    @deltas = @o[:deltas]

    @params = @o[:params] || [
      :l2l3req_tl,
      :l2l3req_o, 
      :l2l3resp_tl, 
      :l2l3resp_o,  
      :l3memreq_tl, 
      :l3memreq_o,  
      :l3memresp_tl,
      :l3memresp_o, 
      :mem_tl,     
      :mem_o,       
    ]

    # @params.each do |p|
    #   eval "instance_variable_set(\"#{p}\",o[:#{p}])"
    # end

  end

  def legal o
    @params.each do |p|
      return false if o[p] > @max[p]
      return false if o[p] < @min[p]
    end
    return true
  end

  # Generates a neighbor of this state by randomly selecting a parameter and 
  # then changing it by a randomly selected delta
  def neighbor &block
    factory = block || lambda { |o| State.new o }
    new_params = @o.merge(
      (p = @params.sample) => @o[p] + @deltas.call(p).sample
    )
    legal(new_params)?
      factory.call(new_params) :
      neighbor
  end

  # Lower energy -> more likely to accept.
  # Suitable options: 
  # EV(l3 hit latency)
  # EV(l3 miss latency)
  # l3 mss latency constrained to best l3 hit latency
  # weighted balance of l3 hit/miss latency
  def energy
    (lat = l3_miss_latencies @o).inject{ |a,l| a+=l }/lat.size.to_f
  end

  #Randomly generate a new legal 
  def shuffle &block
    factory = block || lambda { |o| State.new o }
    factory.call @o.merge(
      @params.inject({}) do |params,p|
        params[p] = @min[p].upto(@max[p]).to_a.sample
        params
      end 
    )
  end

  def to_s
    @params.inject({}){ |h,p| h[p] = @o[p]; h }.to_s
  end

end

class MaximizingState < State
  def energy
    super * -1
  end

  def shuffle
    super{ |o| MaximizingState.new o }
  end

  def neighbor
    super{ |o| MaximizingState.new o}
  end
end

class HitState < State
  def initialize o={}
    params = o[:params] || [
      :l2l3req_tl,
      :l2l3req_o, 
      :l2l3resp_tl, 
      :l2l3resp_o,  
    ]

    super o.merge( params: params )
  end
end

