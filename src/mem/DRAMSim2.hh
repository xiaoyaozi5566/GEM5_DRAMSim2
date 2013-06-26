/*
 * Copyright (c) 2003-2004 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Ron Dreslinski
 *          Ali Saidi
 */

/**
 * @file
 * Derrive a class from PhysicalMemory that support DRAM like timing access.
 */
#ifndef __DRAMSIM2_HH__
#define __DRAMSIM2_HH__

//#include "base/statistics.hh"
#include "mem/DRAMSim2Wrapper.hh"
#include "params/DRAMSim2.hh"

#include "MultiChannelMemorySystem.h"

/**********************************************************************/
// the following parameters are claimed globally because they have to...

// dramsim is the pointer of DRAMSim2 simulator, this pointer is used in 
// src/sim/simulate.cc
DRAMSim::MultiChannelMemorySystem *dramsim2;

// SHOW_SIM_OUT is required by DRAMSim2
int SHOW_SIM_OUTPUT = 1;
/**********************************************************************/

typedef struct {
    PacketPtr pkt;
    void* port;
} AccessMetaInfo;

class DRAMSim2 : public DRAMSim2Wrapper
{
  public:
    typedef DRAMSim2Params Params;
    DRAMSim2(const Params *p);

    const Params *
    params() const
    {
        return dynamic_cast<const Params *>(_params);
    }
  
    // DRAMSim2 Required
    std::multimap<uint64_t, AccessMetaInfo> ongoingAccess;
    void read_complete(unsigned, uint64_t, uint64_t);
    void write_complete(unsigned, uint64_t, uint64_t);
    void report_power(double, double, double, double);
};

#endif// __DRAMSIM2_HH__
