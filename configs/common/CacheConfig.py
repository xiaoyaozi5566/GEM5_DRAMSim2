# Copyright (c) 2010 Advanced Micro Devices, Inc.
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
# Authors: Lisa Hsu

# Configure the M5 cache hierarchy config in one place
#

import m5
from m5.objects import *
from Caches import *
from O3_ARM_v7a import *

class L3Config(object):
    def __init__( self, options, system ):
        self.options = options
        self.system = system
        self.latencies = {
            '4MB' : '8.48ns',
            '3MB' : '7.5',
            '2MB' : '6.5ns',
            '1MB' : '5ns'
        }

    def connect_l2( self ): return

class L3Shared( L3Config ):
    def __init__( self, options, system ):
        super( L3Shared, self ).__init__( options, system )
        system.l3 = L3Cache(size = options.l3_size, 
                            latency=self.latencies[options.l3_size],
                            assoc = options.l3_assoc,
                            block_size=options.cacheline_size,
                            use_set_part = options.use_set_part,
                            num_tcs = options.numpids,
                            use_way_part = options.use_way_part,
                            split_mshrq = options.split_mshr,
                            split_rport = options.split_rport,
                            save_trace = options.do_cache_trace,
                            l3_trace_file = options.l3tracefile)

        system.tol3bus = ( 
                RR_NoncoherentBus(num_pids = options.numpids,
                            req_tl = options.l2l3req_tl,
                            req_offset = options.l2l3req_offset,
                            resp_tl = options.l2l3resp_tl,
                            save_trace = options.do_bus_trace,
                            bus_trace_file = options.bustracefile,
                            resp_offset = options.l2l3resp_offset) if options.rr_nc
                else NoncoherentBus()
                )
        # system.tol3bus = NoncoherentBus()
        system.l3.cpu_side = system.tol3bus.master
        system.l3.mem_side = system.membus.slave

    def connect_l2( self ):
        for i in xrange( self.options.num_cpus ):
            self.system.l2[i].mem_side = self.system.tol3bus.slave



class L3Private( L3Config ):
    def __init__( self, options, system ):
        super( L3Private , self ).__init__( options, system )
        system.l3 = [
                L3Cache(
                    size = options.l3_size,
                    latency = self.latencies[options.l3_size],
                    assoc = options.l3_assoc,
                    block_size = options.cacheline_size,
                    use_set_part = options.use_set_part,
                    use_way_part = options.use_way_part,
                    save_trace = options.do_cache_trace,
                    l3_trace_file = options.l3tracefile
                )
                for i in xrange( options.num_cpus )
            ]

        system.tol3bus = [NoncoherentBus() for i in xrange( options.num_cpus ) ]

        for i in xrange( options.num_cpus ):
            system.l3[i].cpu_side = system.tol3bus[i].master
            system.l3[i].mem_side = system.membus.slave

    def connect_l2( self ):
        for i in xrange( self.options.num_cpus ):
            self.system.l2[i].mem_side = self.system.tol3bus[i].slave


def config_cache(options, system):

    #-------------------------------------------------------------------------
    # L3
    #-------------------------------------------------------------------------
    if options.l3cache:
        if options.l3config == "shared":
            l3config = L3Shared( options, system )
        else:
            l3config = L3Private( options, system )

    #-------------------------------------------------------------------------
    # L1
    #-------------------------------------------------------------------------
    for i in xrange(options.num_cpus):
        if options.caches:
            icache = L1Cache(size = options.l1i_size,
                             assoc = options.l1i_assoc,
                             block_size=options.cacheline_size)
            dcache = L1Cache(size = options.l1d_size,
                             assoc = options.l1d_assoc,
                             block_size=options.cacheline_size)

            if buildEnv['TARGET_ISA'] == 'x86':
                system.cpu[i].addPrivateSplitL1Caches(icache, dcache,
                                                      PageTableWalkerCache(),
                                                      PageTableWalkerCache())
            else:
                system.cpu[i].addPrivateSplitL1Caches(icache, dcache)
        system.cpu[i].createInterruptController()

    #-------------------------------------------------------------------------
    # L2
    #-------------------------------------------------------------------------
    system.l2 = [ 
            L2Cache( 
                size = options.l2_size,
                assoc = options.l2_assoc,
                block_size=options.cacheline_size 
            ) 
            for i in xrange( options.num_cpus )
        ]
    system.tol2bus = [NoncoherentBus() for i in xrange( options.num_cpus )]

    for i in xrange(options.num_cpus):
        if options.l2cache:
            system.cpu[i].connectAllPorts(system.tol2bus[i])
            system.l2[i].cpu_side = system.tol2bus[i].master
            if not options.l3cache:
                system.l2[i].mem_side = system.membus.slave
        else:
            system.cpu[i].connectAllPorts(system.membus)

    if options.l3cache:
        l3config.connect_l2()

    return system
