Dir['*rb'].each { |f| require_relative f }
require 'colored'
require 'parallel_each'

class HillClimber

  def initialize o={}
    @ops = o
    @min = Float::INFINITY

    @params = [
      :@l2l3req_tl,
      :@l2l3req_o, 
      :@l2l3resp_tl, 
      :@l2l3resp_o,  
      :@l3memreq_tl, 
      :@l3memreq_o,  
      :@l3memresp_tl,
      :@l3memresp_o, 
      :@mem_tl,     
      :@mem_o,       
    ]

    @stepper   = o[:stepper]

    @params.each do |p|
      eval "instance_variable_set(\"#{p}\",o[:#{p}])"
    end

  end

  def hill_climb
    step = 0

    @ops[:maxsteps].times do
      step +=1
      break unless find_direction (take_step &@stepper)
    end

    puts "min = #{@min}"
    @params.each do |p|
      puts "#{p} = #{instance_variable_get(p)}"
    end
    puts "step = #{step}"
  end

  def ps param_symbol
    param_symbol.to_s.delete("@").intern
  end

  def take_step
    r = []
    [-1,1].product(@params).each do |step,pi|
      new_p = instance_variable_get(pi) + step
      next if step_is_unsafe pi, new_p
      o = @ops.merge @params.inject({}){ |hsh,pj| 
        hsh[ps(pj)] = instance_variable_get(pj); hsh
      }.merge(
        ps(pi) => new_p,
      )
      r << [yield(o), pi, new_p]
    end
    r
  end

  def find_direction r
    r.each do |result,param,p_val|
      if result < @min
        instance_variable_set(param,p_val)
        @min = result
        return true
      end
    end
    return false
  end

end

def step_is_unsafe param, val
  return true if val < 0
  return true if param.to_s.include?("_tl") && val < 1
  return true if param.to_s.include?("mem_tl") && val < 44
  return false
end


if __FILE__ == $0
  stepper = Proc.new { |o|
    avg(l3_miss_latencies o)
  }
  HillClimber.new(
    stepper: stepper,
    maxsteps: 80,
    :@l2l3req_tl  => (rand()*13).ceil, #1,
    :@l2l3req_o   => (rand()*13).ceil, #0,
    :@l2l3resp_tl => (rand()*13).ceil, #9,
    :@l2l3resp_o  => (rand()*13).ceil, #0,
    :@l3memreq_tl => (rand()*13).ceil, #1,
    :@l3memreq_o  => (rand()*13).ceil, #0,
    :@l3memresp_tl=> (rand()*13).ceil, #9,
    :@l3memresp_o => (rand()*13).ceil, #0,
    :@mem_o       => (rand()*13).ceil, #0,
    :@mem_tl      => (rand()*30).ceil + 44 #4.ceil4 #64,
  ).hill_climb

end
