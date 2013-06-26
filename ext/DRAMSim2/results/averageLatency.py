#!/usr/bin/python2.7

import re, os 
import string
import sys
import array
import random
from random import choice

inputFile = "t0trace"
filenames = ["result_LOW", "result_MEDIUM", "result_HIGH"]
filenames_npo = ["result_LOW_npo", "result_MEDIUM_npo", "result_HIGH_npo"]
filenames_npc = ["result_LOW_npc", "result_MEDIUM_npc", "result_HIGH_npc"]

# count the lines
file = open(filenames[0], "r")
count = 0.00
for line in file:
  count = count + 1
file.close()
print count

file = open(inputFile, "r")
arriveTime = 0
for i in range(0, int(count)):
  line = file.readline()
  time = int(''.join(line.split()[-1:]))
  arriveTime = arriveTime + time
file.close()

file = open(filenames[0], "r")
returnTime = 0
for line in file:
  time = int(''.join(line.split()[-1:]))
  returnTime = returnTime + time
file.close()

file = open(filenames_npc[0], "r")
returnTime_npc_L = 0
for line in file:
  time = int(''.join(line.split()[-1:]))
  returnTime_npc_L = returnTime_npc_L + time
file.close()

file = open(filenames_npc[1], "r")
returnTime_npc_M = 0
for line in file:
  time = int(''.join(line.split()[-1:]))
  returnTime_npc_M = returnTime_npc_M + time
file.close()

file = open(filenames_npc[2], "r")
returnTime_npc_H = 0
for line in file:
  time = int(''.join(line.split()[-1:]))
  returnTime_npc_H = returnTime_npc_H + time
file.close()

file = open(filenames_npo[0], "r")
returnTime_npo_L = 0
for line in file:
  time = int(''.join(line.split()[-1:]))
  returnTime_npo_L = returnTime_npo_L + time
file.close()

file = open(filenames_npo[1], "r")
returnTime_npo_M = 0
for line in file:
  time = int(''.join(line.split()[-1:]))
  returnTime_npo_M = returnTime_npo_M + time
file.close()

file = open(filenames_npo[2], "r")
returnTime_npo_H = 0
for line in file:
  time = int(''.join(line.split()[-1:]))
  returnTime_npo_H = returnTime_npo_H + time
file.close()

print "Average Latency: \n\n"
print "With protection:" + str((returnTime - arriveTime)/count) + "\n"
print "Without protection, Close page, Low demand:" + str((returnTime_npc_L - arriveTime)/count) + "\n"
print "Without protection, Close page, Medium demand:" + str((returnTime_npc_M - arriveTime)/count) + "\n"
print "Without protection, Close page, High demand:" + str((returnTime_npc_H - arriveTime)/count) + "\n"
print "Without protection, Open page, Low demand:" + str((returnTime_npo_L - arriveTime)/count) + "\n"
print "Without protection, Open page, Medium demand:" + str((returnTime_npo_M - arriveTime)/count) + "\n"
print "Without protection, Open page, High demand:" + str((returnTime_npo_H - arriveTime)/count) + "\n"
