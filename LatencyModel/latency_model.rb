Dir['*rb'].each { |f| puts f; require_relative f }
require 'colored'

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
    # Test case
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

def l3_hit_latencies o={}
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
    t_l3:   9,
    t_req:  1,
    t_resp: 9,
    ntcs:   2,
  }.merge o

  ([
    o[:l2l3req_tl],
    o[:l2l3resp_tl],
  ].reduce(:lcm) * o[:ntcs]).times.to_a.map do |time|
    puts ("="*80).blue if DEBUG
    puts (" "*40 + "#{time}").blue if DEBUG
    puts ("="*80).blue if DEBUG
    latency = simulate_l3_hit(o.merge(time: time)) - time
    puts latency.to_s.blue if DEBUG
    latency
  end
end

def simulate_l3_miss o = {}
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
    mem_tl:      64,
    mem_o:        0,
    # Environment
    t_l3:   9,
    t_req:  1,
    t_resp: 9,
    mem_wc: 43,
    mem_wc_read: 24,
    # Test case
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
    l3 = PipelinedCache.new_m(
      name: "L3",
      t_l3:  o[:t_l3]
    ),
    BusLayer.new_m(
      name:    "l3mem_req",
      message: o[:t_req],
      ntcs:    o[:ntcs],
      turn_l:  o[:l3memreq_tl],
      offset:  o[:l3memreq_o],
    ),
    MemoryController.new_m(
      name:    "MemCtl",
      wc:      o[:mem_wc],
      wc_read: o[:mem_wc_read],
      ntcs:    o[:ntcs],
      t_mem:   o[:mem_tl],
      offset:  o[:mem_o]
    ),
    BusLayer.new_m(
      name:    "l3mem_resp",
      message: o[:t_resp],
      ntcs:    o[:ntcs],
      turn_l:  o[:l3memresp_tl],
      offset:  o[:l3memresp_o],
    ),
    l3, # <-- same L3 as above
    BusLayer.new_m(
      name:    "l2l3_resp",
      message: o[:t_resp],
      ntcs:    o[:ntcs],
      turn_l:  o[:l2l3resp_tl],
      offset:  o[:l2l3resp_o],
    )
  ].inject(o[:time]) do |time, component|
    component.update time #happens automatically, needed for print
    puts component.to_s.green if DEBUG && BOXES
    time = component.cycle_complete time
  end
end

def l3_miss_latencies o={}
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
    mem_tl:      64,
    mem_o:        0,
    # Environment
    t_l3:    9,
    t_req:   1,
    t_resp:  9,
    mem_wc: 43,
    ntcs:    2,
  }.merge o

  ([
    o[:l2l3req_tl],
    o[:l2l3resp_tl],
    o[:l3memreq_tl],
    o[:l3memresp_tl],
    o[:mem_tl]
  ].reduce(:lcm) * o[:ntcs]).times.to_a.map do |time|
    puts ("="*80).blue if DEBUG
    puts (" "*40 + "#{time}").blue if DEBUG
    puts ("="*80).blue if DEBUG
    latency = simulate_l3_miss(o.merge(time: time)) - time
    puts latency.to_s.yellow if DEBUG
    latency
  end
end

def avg arr
  arr.reduce(:+)/arr.length.to_f
end

if __FILE__ == $0
  DEBUG = true
  BOXES = false
  SHIFT = true 
  # a = SlotBusLayer.new(ntcs: 4, offset: 0, turn_l: 3, name: "l2l3_req")
  # puts a.cycle_complete 0, 3

  # puts simulate_l3_hit(
  #   time: 0
  # ).to_s.magenta

  # puts simulate_l3_hit(
  #   time: 0,
  #   l2l3req_tl: 1,
  #   l2l3resp_o: 1+9 #t_req + t_l3
  # ).to_s.magenta

  # puts simulate_l3_hit(
  #   time: 0,
  #   l2l3req_tl: 9,
  #   t_req: 2,
  #   l2l3resp_o: 1+9 #t_req + t_l3
  # ).to_s.magenta

  # puts simulate_l3_miss(
  #   l2l3resp_o: 10,
  #   l3memreq_o: 10,
  #   mem_o:      11,
  #   l3memresp_o: 11+24
  # ).to_s.magenta
 
  hit_latencies = l3_miss_latencies(
    l3memreq_o:        10,
    mem_o:             11,
    l3memresp_o:    11+24,
    l2l3resp_o:   11+24+9,
    mem_tl:            64,
  ) 

  puts "|L| = #{avg hit_latencies}".to_s.magenta
  puts "#{hit_latencies.max } < L < #{hit_latencies.min}".to_s.magenta

end
