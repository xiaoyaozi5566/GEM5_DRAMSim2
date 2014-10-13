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

import m5
from m5.defines import buildEnv
from m5.objects import *
from m5.util import addToPath, fatal

addToPath('../common')
addToPath('../ruby')
addToPath('../topologies')

import Options
import Ruby
import Simulation
import CacheConfig
from math import *
from Caches import *
from cpu2000 import *

###############################################################################
## Options
###############################################################################
def add_options():
    parser = optparse.OptionParser()
    Options.addCommonOptions(parser)
    Options.addSEOptions(parser)
    parser.add_option("--dramsim2", action="store_true")
    if '--dramsim2' in sys.argv:
        parser.add_option("--devicecfg", type="string", default="",
                help="device configuration file to be used by DRAMSim2")
        parser.add_option("--systemcfg", type="string", default="", 
                help="system configuration file to be used by DRAMSim2")
        parser.add_option("--tpturnlength", type="string", default="12",
                help="Turn length for TP. Unused if another scheme is used.")
        parser.add_option("--outputfile", type="string", default="",
                help="output file for DRAMSim results."),
        parser.add_option("--fixaddr", action="store_true", default=False,
        		help="fixed the address mapping of each application")
        parser.add_option("--diffperiod", action="store_true", default=False,
        		help="use different periods for different security domains")
        parser.add_option("--p0period", type="int", default=64,
        		help="period for security domain 0")
        parser.add_option("--p1period", type="int", default=64,
        		help="period for security domain 1")
        parser.add_option("--dramoffset", type="int", default=0,
        		help="dram offset")
        # bus turn length and offset
        parser.add_option("--l2l3req_tl", type="int", default=1,
                help="l2l3 bus request layer turn length")
        parser.add_option("--l2l3req_offset", type="int", default=0,
                help="l2l3 bus request layer offset")
        parser.add_option("--l2l3resp_tl", type="int", default=1,
                help="l2l3 bus response layer turn length")
        parser.add_option("--l2l3resp_offset", type="int", default=0,
                help="l2l3 bus response layer offset")
        parser.add_option("--membusreq_tl", type="int", default=1,
                help="membus request layer turn length")
        parser.add_option("--membusreq_offset", type="int", default=0,
                help="membus request layer offset")
        parser.add_option("--membusresp_tl", type="int", default=1,
                help="membus response layer turn length")
        parser.add_option("--membusresp_offset", type="int", default=0,
                help="membus response layer offset")
                
        parser.add_option("--p0", type="string", 
                help="workload for processor 0."),
        parser.add_option("--p0threadID", type="int", default=0,
                help="timing compartment id for p0")
        parser.add_option("--p1", type="string",
                help="workload for processor 1.")
        parser.add_option("--p1threadID", type="int", default=1,
                help="timing compartment id for p1")
        parser.add_option("--p2",type="string", default="echo \"no p2!\"",
                help="workload for processor 2, default is an echo")
        parser.add_option("--p2threadID", type="int", default=2,
                help="timing compartment id for p2")
        parser.add_option("--p3",type="string", default="echo \"no p3!\"",
                help="workload for processor 3, default is an echo")
        parser.add_option("--p3threadID", type="int", default=3,
                help="timing compartment id for p3")
        parser.add_option("--gentrace", action="store_true", default=False,
                help="generate the trace for benchmarks.")
        parser.add_option("--numpids", type="int", default=2,
                help="determine the number of PIDs")
        parser.add_option("--numcpus", type="int", default=None,
                help="set the number of cpus if different from PIDs")
        parser.add_option("--l3tracefile", type="string", default="l3trace.txt",
                help="Output file for l3 cache traces")
        parser.add_option("--l2tracefile", type="string", default="l2trace.txt",
                help="Output file for l2 cache traces")
        parser.add_option("--use_set_part", action="store_true", default=False,
                help="Determines if the L3 cache should be set partitioned")
        parser.add_option("--use_way_part", action="store_true", default=False,
                help="Determines if the L3 cache should be way partitioned")
        parser.add_option("--rr_nc", action="store_true", default=False,
                help="Should a round robin noncoherent bus be used?" )
        parser.add_option("--rr_l2l3", action="store_true", default=False,
                help="Should a round robin noncoherent bus be used for l2l3?" )
        parser.add_option("--rr_mem", action="store_true", default=False,
                help="Should a round robin noncoherent bus be used for membus?" )
        parser.add_option("--split_mshr", action="store_true", default=False,
                help="Determines if L3 has separate MSHR Queues per TC")
        parser.add_option("--split_rport", action="store_true", default=False,
                help="Determines if L3 has separate Response Port Queues per TC")
        parser.add_option("--do_cache_trace", action="store_true", default=False,
                help="Determines if cache traces should be saved and reported")
        parser.add_option("--do_bus_trace", action="store_true", default=False,
                help="Save bus traces or not" )
        parser.add_option("--membustracefile", type="string", default="bustrace.txt",
                help="Output file for bus traces")
        parser.add_option("--l2l3bustracefile", type="string", default="bustrace.txt",
                help="Output file for bus traces")
        parser.add_option("--do_mem_trace", action="store_true", default=False,
                help= "do memory trace" )
        parser.add_option("--mem_trace_file", type="string", default="memtrace.txt",
                help="memory trace file")
        parser.add_option("--addr_trace", action="store_true", default=False,
                help="do detailed trace for address")
        parser.add_option("--trace_addr", type="int", default=0,
                help="address for detailed trace")
        parser.add_option("--nocwf", action="store_true", default=False,
                help="Enable to turn off critical word first timing")

        (options, args) = parser.parse_args()

        if '--ruby' in sys.argv:
            Ruby.define_options(parser)

        # Number of CPUs
        options.num_cpus = ( options.numpids if ( options.numcpus == None )
                else options.numcpus )
        
        #Allow rr_nc to apply rr to both buses
        if options.rr_nc :
            options.rr_l2l3 = True 
            options.rr_mem  = True

        if args:
            print "Error: script doesn't take any positional arguments"
            sys.exit(1)
        return options


###############################################################################
# DRAM
###############################################################################
def setup_dramsim(options):
    np = options.num_cpus

    # num_cpus rounded up to nearest power of 2
    p2cores = int(pow( 2, ceil(log(np)/log(2)) ))
    memorysize = str(1024*p2cores) + 'MB'
    
    ### instantiate the DRAMSim2 model if we enable it
    if options.dramsim2 :
        DRAM = DRAMSim2(    # memory capacity
                            range = AddrRange(memorysize),
                            # cpu clock to do synchronization
                            cpu_clock=options.clock,
                            # device (timing and power) configure file
                            deviceConfigFile = options.devicecfg,
                            # system (channel number, scheduling policy) configure file
                            systemConfigFile=options.systemcfg,
                            # output file for DRAMSim results
                            outputFile=options.outputfile,
                            #TP Turn Length
                            tpTurnLength=options.tpturnlength,
                            #Generate trace
                            genTrace=options.gentrace,
                            #Number of PIDs
                            numPids=options.numpids,
                            #Use fixed address mapping
                            fixAddr=options.fixaddr,
                            #Use different periods
                            diffPeriod=options.diffperiod,
                            #Period for thread 0
                            p0Period=options.p0period,
                            #Period for thread 1
                            p1Period=options.p1period,
                            #Offset for DRAM turn length
                            offset=options.dramoffset,
                            #Infer split memory ports from split cache ports
                            #TODO give this its own option or infer from scheme
                            split_ports = options.split_rport,
                            save_trace  = options.do_mem_trace,
                            trace_file  = options.mem_trace_file,
                            addr_trace  = options.addr_trace,
                            trace_addr  = options.trace_addr
                        );
    else: # or we just use the original memory model
        DRAM = SimpleMemory( range = AddrRange(memorysize) )
    return DRAM


###############################################################################
# Cache Configuration 
###############################################################################
def setup_cache(options):
    options.l1d_size="32kB"
    options.l1d_assoc=2
    options.l1i_size="32kB"
    options.l1i_assoc=2
    options.l2_size="256kB"
    options.l2_assoc=8
                      

###############################################################################
# Workloads
###############################################################################
def setup_workloads(options):
    multiprocesses = []

    for i in range(options.num_cpus):
        process = LiveProcess()
        exec( "process.cmd = options.p"+str(i)+".split()")
        exec( "process.pid = options.p"+str(i)+"threadID")
        multiprocesses.append(process)
    
    if options.input != "":
        process.input = options.input
    if options.output != "":
        process.output = options.output
    if options.errout != "":
        process.errout = options.errout

    return multiprocesses


###############################################################################
# CPUs
###############################################################################
def setup_cpus(options):
    # By default, set workload to path of user-specified binary
    workloads = options.cmd
    numThreads = 1
    
    if options.cpu_type == "detailed" or options.cpu_type == "inorder":
        #check for SMT workload
        workloads = options.cmd.split(';')
        if len(workloads) > 1:
            process = []
            smt_idx = 0
            inputs = []
            outputs = []
            errouts = []
    
            if options.input != "":
                inputs = options.input.split(';')
            if options.output != "":
                outputs = options.output.split(';')
            if options.errout != "":
                errouts = options.errout.split(';')
    
            for wrkld in workloads:
                smt_process = LiveProcess()
                smt_process.executable = wrkld
                smt_process.cmd = wrkld + " " + options.options
                if inputs and inputs[smt_idx]:
                    smt_process.input = inputs[smt_idx]
                if outputs and outputs[smt_idx]:
                    smt_process.output = outputs[smt_idx]
                if errouts and errouts[smt_idx]:
                    smt_process.errout = errouts[smt_idx]
                process += [smt_process, ]
                smt_idx += 1
        numThreads = len(workloads)
    
    (CPUClass, test_mem_mode, FutureClass) = Simulation.setCPUClass(options)
    CPUClass.clock = options.clock
    CPUClass.numThreads = numThreads;

    return (CPUClass, test_mem_mode, FutureClass)


###############################################################################
# System Bus
###############################################################################
def setup_systembus(options):
    RR_NCBus = RR_NoncoherentBus(
               num_pids = options.numpids,
               save_trace = options.do_bus_trace,
               bus_trace_file = options.membustracefile ,
               req_tl = options.membusreq_tl,
               req_offset = options.membusreq_offset,
               resp_tl = options.membusresp_tl,
               resp_offset = options.membusresp_offset)

    return (RR_NCBus if options.rr_mem else NoncoherentBus() )

