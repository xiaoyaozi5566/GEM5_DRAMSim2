#!/usr/bin/python2.7

import re, os 
import string
import sys
import array
import random

if len(sys.argv) != 2:
  sys.exit("Must specify trace file")


trace_filename = sys.argv[1]
tracefile = open(trace_filename, "r")
outfile = open(trace_filename[:-4] + "_MT.trc","w")

for line in tracefile:
  outfile.write("%s %s\n" % (line[:-1],random.randint(0, 1)))

tracefile.close()
outfile.close()