#!/usr/bin/python2.7

import re, os 
import string
import sys
import array
import random
from random import choice

filenames = ["result_LOW", "result_MEDIUM", "result_HIGH"]
flag = 0

ij_grid = [(i, j) for i in range(0, len(filenames)) for j in range(i+1, len(filenames))]

for i, j in ij_grid:
  fileA = open(filenames[i], "r")
  fileB = open(filenames[j], "r")
  for line in fileA:
	  lineB = fileB.readline();
	  if line != lineB:
	    print "Unmatch found for file" + filenames[i] + " and " + filenames[j] + " at line " + line + "\n"
	    flag = 1
  fileA.close()
  fileB.close()

if flag == 0:
  print "Results are the same!"
