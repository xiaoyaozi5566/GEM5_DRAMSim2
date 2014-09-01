#------------------------------------------------------------------------------
# Coordinate
# Req turn lengths are equal to Resp turn lengths and scaled to match the cache 
# block transfer time. The memory turn length is increased for repeatability. 
# Offsets try to guarantee requests / responses proceed unblocked.
#------------------------------------------------------------------------------
def coordinate o={}
  o = {
    n:    2,                 # number of TCs
    tl3:  (8.43 * 2).ceil, # L3 latency in bus cycles
    h:    8,                 # header in bytes
    d:    64,                 # cache block in bytes
    w:    8,                 # bus width in bytes per bus cycle
    memt: 64                 # optimal memctl turn length
  }.merge o
  n, tl3, h, d, w, memt = o[:n], o[:tl3], o[:h], o[:d], o[:w], o[:memt]

  params ={diffperiod: true}

  # For an unpipleined cache that can tolerate only one read at a time,
  # all bus layer turn lengths are either the l3 latency or the time to 
  # transfer a cache block over the bus - whichever is greater. If the 
  # cache is fully pipelined, it is just time to transfer a cache block.
  # tbus = [tl3,((h+d)/w).ceil].max
  tbus = ((h+d)/w.to_f).ceil
  params[:l2l3req_tl]    = tbus
  params[:l2l3resp_tl]   = tbus
  params[:membusreq_tl]  = tbus
  params[:membusresp_tl] = tbus
  
  #first multiple of y greater than or equal to x 
  first_mul_g = lambda { |x, y| (x%y==0) ? x : x + (y - x % y) }
  # Memory controller turn lengths are the first multiple of the bus turn 
  # length (in memory cycles) multiplied by the number of timing 
  # compartments plus one. It guarantees that the memory controller turn 
  # starts and ends with the same TCID on the l3mem respones layer. This is 
  # done for repeatability and takes advantage of the fact that data is 
  # available before the memory controller turn ends.
  tbus_m = (tbus / 1.5).ceil
  tmem = first_mul_g.call(memt, (tbus_m * (n + 1)).ceil)
  params[:tl0] = tmem
  params[:tl1] = tmem
  params[:tl2] = tmem unless n < 3
  params[:tl3] = tmem unless n < 4

  # Offsets
  params[:l2l3req_offset]    = 0
  params[:l2l3resp_offset]   = (h / w.to_f).ceil + tl3
  params[:membusreq_offset]  = (h / w.to_f).ceil + tl3
  params[:membusresp_offset] = params[:l2l3resp_offset] - tbus - tl3
  params[:dramoffset] = (params[:membusreq_offset]  / 1.5).ceil +
    (h / w / 1.5).ceil

  # All offsets are made into a nonpositive number that is equivalent to 
  # the original modulo the size of a schedule. This is done by simply 
  # subtracting the schedule size from the offset. A bus schedule is 
  # n * tbus bus cycles and a memory schedule is n * tmem memory cycles.
  [
    :l2l3req_offset,
    :l2l3resp_offset,
    :membusreq_offset,
    :membusresp_offset
  ].each { |p| eval "params[:#{p.to_s}] -= #{n * tbus}" }

  params

end

#------------------------------------------------------------------------------
# Coordinate Hits
# Similar to coordinate, but don't increase the memory controller turn length
#------------------------------------------------------------------------------
def coordinate_hits o={}
  o = {
    n:    2,                 # number of TCs
    tl3:  (8.43 * 2).ceil, # L3 latency in bus cycles
    h:    8,                 # header in bytes
    d:    64,                 # cache block in bytes
    w:    8,                 # bus width in bytes per bus cycle
    memt: 64                 # optimal memctl turn length
  }.merge o
  n, tl3, h, d, w, memt = o[:n], o[:tl3], o[:h], o[:d], o[:w], o[:memt]
  
  params = { diffperiod: true }

  tbus = ((h+d)/w.to_f)..ceil
  params[:l2l3req_tl]    = tbus
  params[:l2l3resp_tl]   = tbus
  params[:membusreq_tl]  = tbus
  params[:membusresp_tl] = tbus
 
  tmem = memt 
  params[:tl0] = tmem
  params[:tl1] = tmem
  params[:tl2] = tmem unless n < 3
  params[:tl3] = tmem unless n < 4

  # Offsets
  params[:l2l3req_offset]    = 0
  params[:l2l3resp_offset]   = (h / w.to_f).ceil + tl3
  params[:membusreq_offset]  = (h / w.to_f).ceil + tl3
  params[:membusresp_offset] = params[:l2l3resp_offset] - tbus - tl3
  params[:dramoffset] = (params[:membusreq_offset]  / 1.5).ceil +
    (h / w / 1.5).ceil

  # All offsets are made into a nonpositive number that is equivalent to 
  # the original modulo the size of a schedule. This is done by simply 
  # subtracting the schedule size from the offset. A bus schedule is 
  # n * tbus bus cycles and a memory schedule is n * tmem memory cycles.
  [
    :l2l3req_offset,
    :l2l3resp_offset,
    :membusreq_offset,
    :membusresp_offset
  ].each { |p| eval "params[:#{p.to_s}] -= #{n * tbus}" }
  params[:dramoffset] -= tmem * n

  params
end

def uncoordinate o={}
  o = {
    n:    2,                 # number of TCs
    tl3:  (8.43 * 2).ceil, # L3 latency in bus cycles
    h:    8,                 # header in bytes
    d:    64,                 # cache block in bytes
    w:    8,                 # bus width in bytes per cycle
    memt: 64                 # optimal memctl turn length
  }.merge o
  n, tl3, h, d, w, memt = o[:n], o[:tl3], o[:h], o[:d], o[:w], o[:memt]

  params = { diffperiod: true }

  # Get the same turn lengths as coordinate
  params = coordinate o

  # F@$% up the offsets by shifting back one turn. The request must wait 
  # through the entire schedule for every interaction assuming there is 
  # repeatability.
  [
    :l2l3req_offset,
    :l2l3resp_offset,
    :membusreq_offset,
    :membusresp_offset
  ].each { |p| eval "params[:#{p.to_s}] -= #{tbus}" }
  params[:dramoffset] -= tmem * n

  params
end
