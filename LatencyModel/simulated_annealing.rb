Dir['*rb'].each { |f| require_relative f }
require 'colored'

def acceptance_probability e, e_prime, temp
  return 1.0 if e_prime < e
  d = (e-e_prime)/e.to_f * 25
  r = Math.exp(d * (1.0-temp))
  puts "P=#{r}".to_s.yellow if DEBUG_S
  r
end

def temperature time
  0.95 ** time
end

def simulate_annealing o={}

  time = 0 
  total_time = 0
  max_time = 400
  best_state = nil
  best_energy = Float::INFINITY
  restart_probability = 0.001
  current_state = State.new.shuffle

  while total_time < max_time

    if DEBUG_S
      puts "=" * 80
      puts " "*38 + "#{time}" + " "*38
      puts "=" * 80
    end

    new_state = if rand < restart_probability
      State.new.shuffle
      time = 0
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

    time+=1
    total_time+=1

  end

  puts "best energy #{best_energy}"
  puts "best_state #{best_state}"

end

if __FILE__ == $0
  DEBUG_S = false #true
  #DEBUG = true
  simulate_annealing
end
