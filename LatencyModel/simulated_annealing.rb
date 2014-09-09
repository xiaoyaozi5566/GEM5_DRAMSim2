Dir['*rb'].each { |f| require_relative f }
require 'colored'

def acceptance_probability e, e_prime, temp
  return 1.0 if e_prime < e
  d = (e-e_prime)/e.to_f * 25
  r = Math.exp(-1*(d.abs) * (1.0-temp))
  puts "P=#{r}".to_s.yellow if DEBUG_S
  r
end

def temperature time
  0.95 ** time
end

def simulate_annealing o={}
  o = {
    shuffle: lambda { State.new.shuffle },
    init: lambda { State.new.shuffle},
    max_time: 400
  }.merge o

  time = 0 
  total_time = 0
  max_time = o[:max_time] 
  best_state = nil
  best_energy = Float::INFINITY
  restart_probability = 0.001
  best_state = current_state = o[:init].call
  best_energy = current_state.energy

  while total_time < max_time

    if DEBUG_S
      puts "=" * 80
      puts " "*38 + "#{time}" + " "*38
      puts "=" * 80
    end

    new_state = if rand < restart_probability
      time = 0
      o[:shuffle].call
    else
      current_state.neighbor
    end

    if DEBUG_S
      puts current_state.to_s.blue
      puts new_state.to_s.magenta
    end

    e, e_prime = best_energy, new_state.energy
    temp = temperature(time)

    if DEBUG_S
      puts "e = #{e}".to_s.blue
      puts "e_prime = #{e_prime}".to_s.magenta
    end

    if rand < acceptance_probability( e, e_prime, temp )
      puts "ACCEPT".green if DEBUG_S
      current_state = new_state
    end

    if e_prime < best_energy
      best_energy = e_prime
      best_state = new_state
    end

    puts "best_energy = #{best_energy}".red
    puts best_state.to_s.red

    time += 1
    total_time += 1

  end

  puts "best energy #{best_energy}"
  puts "best_state #{best_state}"

end

if __FILE__ == $0
  DEBUG_S = true

  simulate_annealing(
    max_time: 20,
    init: lambda do
      BalancedHitState.new(
        l2l3req_tl:                11,
        l2l3resp_tl:               11,
        l3memreq_tl:               66,
        l3memresp_tl:              66,
        mem_tl:                    66,
        l2l3req_o:                  0,
        l3memreq_o:                10,
        mem_o:                   10+1,
        l3memresp_o:          10+1+36,
        l2l3resp_o: (10+11+36+9+9)%11,
      )
    end,
    shuffle: lambda { BalancedHitState.new.shuffle }
  )

  #simulate_annealing( shuffle: lambda {MaximizingState.new.shuffle}, max_time: 5000 )
end
