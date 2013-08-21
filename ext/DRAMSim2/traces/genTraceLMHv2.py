#!/usr/bin/python2.7

import re, os 
import string
import sys
import array
import random
from random import choice

if len(sys.argv) != 2:
  sys.exit("Must specify trace file")


trace_filename = sys.argv[1]
tracefile = open(trace_filename, "r")
outfile_low = open(trace_filename[:-4] + "_MT_LOW.trc","w")
outfile_medium = open(trace_filename[:-4] + "_MT_MEDIUM.trc","w")
outfile_high = open(trace_filename[:-4] + "_MT_HIGH.trc","w")
outfile_higher = open(trace_filename[:-4] + "_MT_HIGHER.trc","w")

prev_time = 0

for i in range(0, 10000):
  address = "0x" +choice(['1', '2', '4']) + "{0:0{1}x}".format(random.randint(0, 1048575), 5) + choice(['0', '4', '8', 'c']) + '0'
  command = choice(["READ", "WRITE", "IFETCH"])
  time = prev_time + random.randint(5, 200)
  
  low = random.randint(0, 1)
  medium = random.randint(0, 2)
  high = random.randint(0, 3)
  higher = random.randint(0, 4)
  
  prev_time_low = prev_time
  for x in range(0,low):
    address_low = "0x" +choice(['1', '2', '4']) + "{0:0{1}x}".format(random.randint(0, 1048575), 5) + choice(['0', '4', '8', 'c']) + '0'
    command_low = choice(["READ", "WRITE", "IFETCH"])
    time_low = random.randint(prev_time_low + 1, time - low + x)
    outfile_low.write("%s %s %s %s\n" % (address_low, command_low, time_low, 1))
    prev_time_low = time_low
  outfile_low.write("%s %s %s %s\n" % (address, command, time, 0))
  
  prev_time_medium = prev_time
  for y in range(0,medium):
    address_medium = "0x" +choice(['1', '2', '4']) + "{0:0{1}x}".format(random.randint(0, 1048575), 5) + choice(['0', '4', '8', 'c']) + '0'
    command_medium = choice(["READ", "WRITE", "IFETCH"])
    time_medium = random.randint(prev_time_medium + 1, time - medium + y)
    outfile_medium.write("%s %s %s %s\n" % (address_medium, command_medium, time_medium, 1))
    prev_time_medium = time_medium
  outfile_medium.write("%s %s %s %s\n" % (address, command, time, 0))

  prev_time_high = prev_time
  for z in range(0,high):
    address_high = "0x" +choice(['1', '2', '4']) + "{0:0{1}x}".format(random.randint(0, 1048575), 5) + choice(['0', '4', '8', 'c']) + '0'
    command_high = choice(["READ", "WRITE", "IFETCH"])
    time_high = random.randint(prev_time_high + 1, time - high + z)
    outfile_high.write("%s %s %s %s\n" % (address_high, command_high, time_high, 1))
    prev_time_high = time_high
  outfile_high.write("%s %s %s %s\n" % (address, command, time, 0))

  prev_time_higher = prev_time
  for z in range(0,higher):
    address_higher = "0x" +choice(['1', '2', '4']) + "{0:0{1}x}".format(random.randint(0, 1048575), 5) + choice(['0', '4', '8', 'c']) + '0'
    command_higher = choice(["READ", "WRITE", "IFETCH"])
    time_higher = random.randint(prev_time_higher + 1, time - higher + z)
    outfile_higher.write("%s %s %s %s\n" % (address_higher, command_higher, time_higher, 1))
    prev_time_higher = time_higher
  outfile_higher.write("%s %s %s %s\n" % (address, command, time, 0))

  prev_time = time
  
tracefile.close()
outfile_low.close()
outfile_medium.close()
outfile_high.close()
outfile_higher.close()
