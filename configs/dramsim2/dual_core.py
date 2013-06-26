# Copyright (c) 2012 ARM Limited
# All rights reserved.
#
# The license below extends only to copyright in the software and shall
# not be construed as granting a license to any other intellectual
# property including but not limited to intellectual property relating
# to a hardware implementation of the functionality of the software
# licensed hereunder.  You may use the software subject to the license
# terms below provided that you ensure that this notice is replicated
# unmodified and in its entirety in all distributions of the software,
# modified or unmodified, in source code or in binary form.
#
# Copyright (c) 2006-2008 The Regents of The University of Michigan
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Authors: Steve Reinhardt

# Simple test script
#
# "m5 test.py"

import optparse
import sys
import os

import m5
from m5.defines import buildEnv
from m5.objects import *
from m5.util import addToPath, fatal

addToPath('../common')
addToPath('../ruby')

import Options
import Ruby
import Simulation
import CacheConfig
from Caches import *
from cpu2000 import *

parser = optparse.OptionParser()
Options.addCommonOptions(parser)
Options.addSEOptions(parser)

if '--ruby' in sys.argv:
    Ruby.define_options(parser)

(options, args) = parser.parse_args()

if args:
    print "Error: script doesn't take any positional arguments"
    sys.exit(1)

# Number of threads per CPU
numThreads = 1

# Create new CPU type for main core
(MainCPUClass, test_mem_mode, FutureClass) = Simulation.setCPUClass(options)
MainCPUClass.numThreads = numThreads;
MainCPUClass.fifo_enabled = True
MainCPUClass.monitoring_enabled = False
# Enable slack timer so it can write to it
MainCPUClass.timer_enabled = True
# Don't need flag cache for main core
MainCPUClass.flagcache_enabled = False

# Create new CPU type for monitoring core
(MonCPUClass, test_mem_mode, FutureClass) = Simulation.setCPUClass(options)
MonCPUClass.numThreads = numThreads;
# Has port to access fifo, but does not enqueue monitoring events
MonCPUClass.fifo_enabled = True
MonCPUClass.monitoring_enabled = False
# Enable slack timer so it can read from it
MonCPUClass.timer_enabled = True
# Need flag cache for monitoring core
MonCPUClass.flagcache_enabled = True

invalidation_cpu = MonCPUClass

# Set up monitoring filter
execfile( os.path.dirname(os.path.realpath(__file__)) + "/monitors.py" )

# Create system, CPUs, bus, and memory
system = System(cpu = [MainCPUClass(cpu_id=0), MonCPUClass(cpu_id=1)],
                physmem = SimpleMemory(range=AddrRange("512MB"), latency=mem_latency),
                membus = CoherentBus(), mem_mode = test_mem_mode)

# Number of CPUs
options.num_cpus = 2

# Create a "fifo" memory
fifo = Fifo(range=AddrRange(start=0x30000000, size="64kB")) 
# fifo.fifo_size = 32
system.fifo = fifo
# Create timer
timer = Timer(range=AddrRange(start=0x30010000, size="64kB"))
system.timer = timer
# Create flag cache
flagcache = FlagCache(range=AddrRange(start=0x30020000, size="64kB"))
system.flagcache = flagcache

for i in range(options.num_cpus):
  # Connect CPU to fifo
  if system.cpu[i].fifo_enabled:
    system.cpu[i].fifo_port = system.fifo.port
  # Connect CPU to timer
  if system.cpu[i].timer_enabled:
    system.cpu[i].timer_port = system.timer.port
  # Connect CPU to flag cache
  if system.cpu[i].flagcache_enabled:
    system.cpu[i].flagcache_port = system.flagcache.port

# Assign programs
process0 = LiveProcess()
# If passed a program from script options
if options.cmd:
  process0.executable = options.cmd
  process0.cmd = [options.cmd] + options.options.split()
else:
  #process0.executable = os.environ["GEM5"] + "/tests/malarden_monitor/malarden_wcet.arm"
  process0.executable = os.environ["GEM5"] + "/tests/monitoring/timer_monitor.arm"
  # process0.executable = os.environ["GEM5"] + "../../papabench/sw/airborne/autopilot/autopilot.elf"
  process0.cmd = ""
system.cpu[0].workload = process0

process1 = LiveProcess()
process1.executable = os.environ["GEM5"] + ("/tests/monitoring/%s.arm" % monitor_bin)
process1.cmd = ""
system.cpu[1].workload = process1

# Connect system to the bus
system.system_port = system.membus.slave
# Connect memory to bus
system.physmem.port = system.membus.master
# Set up caches if enabled, connect to memory bus, and set up interrupts
CacheConfig.config_cache(options, system)

# Run simulation
root = Root(full_system = False, system = system)
Simulation.run(options, root, system, FutureClass)
