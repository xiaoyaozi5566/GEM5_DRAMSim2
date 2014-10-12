#!/usr/bin/python

import optparse
import sys

import m5
from m5.defines import buildEnv
from m5.objects import *
from m5.util import addToPath, fatal
addToPath('../dramsim2')
import dramsim2_se
from dramsim2_se import *


###############################################################################
# System Configuration
###############################################################################

#All the functions called in this section are in dramsim2_se.py

#Incorporate all the options available do dramsim2_se.py
options = add_options()
#Use DRAMSim for the shared memory
DRAM = setup_dramsim(options)
#Setup benchmarks
multiprocesses = setup_workloads(options)
#Setup CPUs
(CPUClass,test_mem_mode,FutureClass) = setup_cpus(options)

#Use a bus that doesn't model interference
systembus = MagicBus()

system = System(cpu = [CPUClass(cpu_id=i) for i in xrange(options.num_cpus)],
                physmem = DRAM,
                membus =  systembus,
                mem_mode = test_mem_mode,
                numPids = options.numpids,
                fast_forward = (options.fast_forward != None),
                fixAddr = options.fixaddr)

for i in xrange(options.num_cpus):
    system.cpu[i].workload = multiprocesses[i]

    if options.checker:
        system.cpu[i].addCheckerCpu()

system.system_port = system.membus.slave
system.physmem.port = system.membus.master

# Setup L1i and L1d.
# The call to config_l1 builds these caches and then stores them in a stack of 
# caches pending connection. Then, connectAllPorts checks this stack and 
# connects the ports properly.
CacheConfig.config_l1( options, system )
for i in xrange( options.num_cpus ):
    system.cpu[i].connectAllPorts( system.membus )


#Start the simulation
root = Root(full_system = False, system = system)
Simulation.run(options, root, system, FutureClass, options.numpids)
