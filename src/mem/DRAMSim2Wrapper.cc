/*
 * Copyright (c) 2010-2012 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2001-2005 The Regents of The University of Michigan
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
 *          Andreas Hansson
 */

#include "base/random.hh"
#include "mem/DRAMSim2Wrapper.hh"

using namespace std;

DRAMSim2Wrapper::DRAMSim2Wrapper(const Params* p) :
    AbstractMemory(p), numPids(p->numPids),
    lat(p->latency), lat_var(p->latency_var)
{
    port = p->split_ports ?
        new SplitMemoryPort( name() + ".port", this, p->numPids ) :
        new MemoryPort( name() + ".port", this, p->numPids );

}

void
DRAMSim2Wrapper::init()
{
    // allow unconnected memories as this is used in several ruby
    // systems at the moment
    if (port->isConnected()) {
        port->sendRangeChange();
    }
}

Tick
DRAMSim2Wrapper::calculateLatency(PacketPtr pkt)
{
    if (pkt->memInhibitAsserted()) {
        return 0;
    } else {
        Tick latency = lat;
        if (lat_var != 0)
            latency += random_mt.random<Tick>(0, lat_var);
        return latency;
    }
}

Tick
DRAMSim2Wrapper::doAtomicAccess(PacketPtr pkt)
{
    access(pkt);
    return calculateLatency(pkt);
}

void
DRAMSim2Wrapper::doFunctionalAccess(PacketPtr pkt)
{
    functionalAccess(pkt);
}

SlavePort& 
DRAMSim2Wrapper::getSlavePort(const std::string &if_name, int idx)
{
    if (if_name != "port") {
        return MemObject::getSlavePort(if_name, idx);
    } else {
        return *port;
    }
}

unsigned int
DRAMSim2Wrapper::drain(Event *de)
{
    int count = port->drain(de);

    if (count)
        changeState(Draining);
    else
        changeState(Drained);
    return count;
}

DRAMSim2Wrapper::MemoryPort::MemoryPort(const std::string& _name,
                                     DRAMSim2Wrapper* _memory, int numPids)
    : SimpleTimingPort(_name, _memory), memory(_memory)
{ 	
    respQueues = new SlavePacketQueue*[numPids];
    for( int i=0; i < numPids; i++ ){
        respQueues[i] = new SlavePacketQueue( *_memory, *this);
    }
	//printf("memory port is called\n");
}

AddrRangeList
DRAMSim2Wrapper::MemoryPort::getAddrRanges() const
{
    AddrRangeList ranges;
    ranges.push_back(memory->getAddrRange());
    return ranges;
}

Tick
DRAMSim2Wrapper::MemoryPort::recvAtomic(PacketPtr pkt)
{
    return memory->doAtomicAccess(pkt);
}

void
DRAMSim2Wrapper::MemoryPort::recvFunctional(PacketPtr pkt)
{
    pkt->pushLabel(memory->name());

    if (!queue.checkFunctional(pkt)) {
        // Default implementation of SimpleTimingPort::recvFunctional()
        // calls recvAtomic() and throws away the latency; we can save a
        // little here by just not calculating the latency.
        memory->doFunctionalAccess(pkt);
    }

    pkt->popLabel();
}

DRAMSim2Wrapper*
DRAMSim2WrapperParams::create()
{
    return new DRAMSim2Wrapper(this);
}
