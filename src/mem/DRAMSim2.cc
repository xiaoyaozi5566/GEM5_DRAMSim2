/*
 * Copyright (c) 2004 The Regents of The University of Michigan
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
 * Authors: Ali Saidi
 *          Ron Dreslinski
 */

/**
 * DRAMsim v2 integration
 * Author: Rio Xiangyu Dong
 *         Tao Zhang
 */

// #define DEBUGI
#define interesting 0x66df80
#define interesting_era_l 8429720000
#define interesting_era_h 8430182001

#include <cstdlib>
#include <iomanip>

#include "mem/DRAMSim2.hh"

bool
isInteresting(PacketPtr pkt ){
#ifdef interesting
    bool is_interesting_time = 
        (curTick() >= interesting_era_l) &&
        (curTick() <= interesting_era_h);
    bool is_interesting_addr = (pkt->getAddr() == interesting);
    return is_interesting_time || is_interesting_addr;
#else
    return false;
#endif
}

DRAMSim2::DRAMSim2(const Params *p) : DRAMSim2Wrapper(p)
{
    warn("This is an integrated DRAMsim v2 module");
    int memoryCapacity = (int)(params()->range.size() / 1024 / 1024);
    std::cout << "device file: " << p->deviceConfigFile << std::endl;
    std::cout << "system file: " << p->systemConfigFile << std::endl;
    std::cout << "output file: " << p->outputFile << std::endl;
    //dramsim2 = new DRAMSim::MultiChannelMemorySystem(p->deviceConfigFile, p->systemConfigFile, p->cwd, p->traceFile, memoryCapacity, "./results/output", NULL, NULL);
    dramsim2 = new DRAMSim::MultiChannelMemorySystem(p->deviceConfigFile, 
            p->systemConfigFile, atoi((p->tpTurnLength).c_str()), p->genTrace, p->cwd, p->traceFile, 
            memoryCapacity, p->outputFile, NULL, NULL, p->numPids, p->fixAddr, p->diffPeriod, p->p0Period, p->p1Period, p->offset);
    // intentionally set CPU:Memory clock ratio as 1, we do the synchronization later
    dramsim2->setCPUClockSpeed(0);
    num_pids = p->numPids;
    
    std::cout << "CPU Clock = " << (int)(1000000 / p->cpu_clock) << "MHz" << std::endl;
    std::cout << "DRAM Clock = " << (1000 / tCK) << "MHz" << std::endl;
    std::cout << "Memory Capacity = " << memoryCapacity << "MB" << std::endl;

    DRAMSim::Callback_t *read_cb = new DRAMSim::Callback<DRAMSim2, void, unsigned, uint64_t, uint64_t, uint64_t>(this, &DRAMSim2::read_complete);
    DRAMSim::Callback_t *write_cb = new DRAMSim::Callback<DRAMSim2, void, unsigned, uint64_t, uint64_t, uint64_t>(this, &DRAMSim2::write_complete);
    dramsim2->RegisterCallbacks(read_cb, write_cb, NULL);
}

DRAMSim2 *
DRAMSim2Params::create()
{
    return new DRAMSim2(this);
}

bool
DRAMSim2::MemoryPort::recvTimingReq(PacketPtr pkt)
{
    memory->tracePrinter->addTrace( pkt, "recvTimingReq" );
    /* I don't know when they can remove this... */
    /// @todo temporary hack to deal with memory corruption issue until
    /// 4-phase transactions are complete. Remove me later
    //cout << "Cycle: " << dramsim2->currentClockCycle << " Receive Timing Request" << endl;
    this->removePendingDelete();

    DRAMSim2* dram = dynamic_cast<DRAMSim2*>(memory);
    bool retVal = false;

    // if DRAMSim2 is disabled, just use the default recvTiming. 
    if( !dram ) {
        return SimpleTimingPort::recvTimingReq(pkt);
    } else {
#ifdef DEBUGI
        if( isInteresting( pkt ) ){
            printf( "(%8s) using dramsim recvTiming\n",
                std::to_string( pkt->getAddr() ).c_str() );
        }
#endif
        if (pkt->memInhibitAsserted()) {
            // snooper will supply based on copy of packet
            // still target's responsibility to delete packet
            delete pkt;
            return true;
        }

        uint64_t addr = pkt->getAddr();
        AccessMetaInfo meta;
        meta.pkt = pkt;
        meta.port = this;
        TransactionType transType;

        while ((double)dramsim2->currentClockCycle <= (double)(curTick()) / 1000.0 / tCK) {
            dramsim2->update();
#ifdef DEBUGI
            if( isInteresting( pkt ) )
                printf( "(%8s) g5 time is %lu and d2 time is %lu after an update\n",
                        std::to_string( pkt->getAddr() ).c_str(),
                        curTick(),
                        dramsim2->currentClockCycle );
#endif
        }

        if (pkt->needsResponse()) {
#ifdef DEBUGI
            if( isInteresting( pkt ) ){
                printf( "(%8s) interesting needs response\n",
                        std::to_string( pkt->getAddr() ).c_str() );
            }
#endif
            if (pkt->isRead()) {
                transType = DATA_READ;
            } else if (pkt->isWrite()) {
                transType = DATA_WRITE;
            } else {
                // only leverage the atomic access to move data, don't use the
                // latency
                //std::cout << "case 1" << std::endl;
                dram->doAtomicAccess(pkt);
                assert(pkt->isResponse());
                schedTimingResp(pkt, curTick() + 1, pkt->threadID);
                return true;
            }

            // record the READ/WRITE request for later use
	    uint64_t index = addr << 1;
            if (transType == DATA_WRITE)
                index = index | 0x1;
            dram->ongoingAccess.insert(make_pair(index, meta));
            uint64_t threadID = pkt->threadID;
            // For trace generation
            if (threadID >= dram->num_pids){
                fprintf( stderr,"warn: gem5 made a trans for a packet with threadID >= dram->numpids\n" );
                threadID = 0;
            }

#ifdef DEBUGI
            if( isInteresting( pkt ) ){
                printf( "(%8s) made interesting trans: g5 time %lu / d2 time %lu\n",
                        std::to_string( pkt->getAddr() ).c_str(),
                        curTick(), dramsim2->currentClockCycle );
            }
#endif
            Transaction tr = Transaction(transType, addr, NULL, threadID, dramsim2->currentClockCycle, 0);
            retVal = dramsim2->addTransaction(tr);
            //std::cout << "case 2" << std::endl;
            //std::cout << "Thread: " << threadID << std::endl;
            //std::cout << "Addr  : " << std::hex << setfill('0') << setw(8) << addr << std::endl;
        } else {
            if (pkt->isWrite()) {	// write-back does not need a response, but DRAMsim2 needs to track it
                transType = DATA_WRITE;
                uint64_t threadID = pkt->threadID;
                // For trace generation
            	if (threadID >= dram->num_pids) {
                    fprintf( stderr,"warn: gem5 made a trans for a packet with threadID >= dram->numpids\n" );
                    threadID = 0;
                }
#ifdef DEBUGI
                if( isInteresting( pkt ) ){
                    printf( "(%8s) made interesting trans: g5 time %lu / d2 time %lu\n",
                            std::to_string( pkt->getAddr() ).c_str(),
                            curTick(), dramsim2->currentClockCycle );
                }
#endif
                Transaction tr = Transaction(transType, addr, NULL, threadID, dramsim2->currentClockCycle, 0);
                retVal = dramsim2->addTransaction(tr);
                if (retVal == true) {
                	dram->doAtomicAccess(pkt);
                	this->addPendingDelete(pkt);
                }
                //std::cout << "case 3" << std::endl;
                // assert(retVal == true);
            }
            else {
            	//std::cout << "case 4" << std::endl;
				retVal = dram->doAtomicAccess(pkt);

				// Again, I don't know....
				/// @todo nominally we should just delete the packet here.
				/// Until 4-phase stuff we can't because the sending
				/// cache is still relying on it
				this->addPendingDelete(pkt);
			}
        }
    }
    return retVal;
}

void DRAMSim2::read_complete(unsigned id, uint64_t address, uint64_t clock_cycle, uint64_t threadID)
{
	//printf("read complete for %llx @ cycle %llu\n", address, clock_cycle);
    uint64_t index = address << 1;
    if (ongoingAccess.count(index)) {
        AccessMetaInfo meta = ongoingAccess.find(index)->second;
        PacketPtr pkt = meta.pkt;
        DRAMSim2::MemoryPort *my_port = (DRAMSim2::MemoryPort*)(meta.port);

        my_port->removePendingDelete();

        if (pkt->needsResponse()) {
            doAtomicAccess(pkt);
            assert(pkt->isResponse());

            // remove the skew between DRAMSim2 and gem5
            Tick toSchedule = (Tick)(clock_cycle) * (Tick)(tCK * 1000);
#ifdef DEBUGI
            if( isInteresting( pkt ) ){
                printf( "(%8s) toSchedule interesting before calc %lu\n SimClock %lu\n curTick %lu\n",
                        std::to_string( pkt->getAddr() ).c_str(),
                        toSchedule,
                        SimClock::Int::ms,
                        curTick()
                        );
            }
#endif
            if (toSchedule <= curTick()){
#ifdef DEBUGI
                  if ( isInteresting( pkt ) ){
                      printf( "(%8s) (1) taken\n",
                           std::to_string( pkt->getAddr() ).c_str() );
                  }
#endif 
                  toSchedule = curTick() + 1;  //not accurate, but I have to
            }
            if (toSchedule >= curTick() + SimClock::Int::ms){
#ifdef DEBUGI
                  if ( isInteresting( pkt ) ){
                      printf( "(%8s) (2) taken\n",
                           std::to_string( pkt->getAddr() ).c_str() );
                  }
#endif
                  toSchedule = curTick() + SimClock::Int::ms - 1; //not accurate
            }
#ifdef DEBUGI
            if( isInteresting( pkt ) ){
                printf( "(%8s) toSchedule interesting after calc %lu\n",
                        std::to_string( pkt->getAddr() ).c_str(),
                        toSchedule );
            }
#endif
            my_port->schedTimingResp(pkt, toSchedule, threadID);
            tracePrinter->addTrace( pkt, "read_complete", toSchedule );
        } else {
            my_port->addPendingDelete(pkt);
        }
        ongoingAccess.erase(ongoingAccess.find(index));
    }
}

void DRAMSim2::write_complete(unsigned id, uint64_t address, uint64_t clock_cycle, uint64_t threadID)
{
    uint64_t index = address << 1;
    index = index | 0x1;
    if (ongoingAccess.count(index)) {
        AccessMetaInfo meta = ongoingAccess.find(index)->second;
        PacketPtr pkt = meta.pkt;
        DRAMSim2::MemoryPort *my_port = (DRAMSim2::MemoryPort*)(meta.port);

        my_port->removePendingDelete();

        if (pkt->needsResponse()) {
            doAtomicAccess(pkt);
            assert(pkt->isResponse());
            Tick toSchedule = (Tick)(clock_cycle) * (Tick)(tCK * 1000);
#ifdef DEBUGI
            // if( isInteresting( pkt ) ){
            //     printf( "toSchedule interesting before calc %lu\n", toSchedule );
            // }
#endif
            if (toSchedule <= curTick())
                  toSchedule = curTick() + 1;  //not accurate, but I have to
            if (toSchedule >= curTick() + SimClock::Int::ms)
                  toSchedule = curTick() + SimClock::Int::ms - 1; //not accurate
#ifdef DEBUGI
            // if( isInteresting( pkt ) ){
            //     printf( "toSchedule interesting after calc %lu\n", toSchedule );
            // }
#endif
            my_port->schedTimingResp(pkt, toSchedule, threadID);
            tracePrinter->addTrace( pkt, "write_complete", toSchedule );
        } else {
            my_port->addPendingDelete(pkt);
        }
        ongoingAccess.erase(ongoingAccess.find(index));
    }
}

void DRAMSim2::report_power(double a, double b, double c, double d)
{
}


