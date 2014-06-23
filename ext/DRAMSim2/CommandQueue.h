/*********************************************************************************
 *  Copyright (c) 2010-2011, Elliott Cooper-Balis
 *                             Paul Rosenfeld
 *                             Bruce Jacob
 *                             University of Maryland dramninjas [at] gmail 
 *                             [dot] com
 *  All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  
 *     * Redistributions of source code must retain the above copyright notice,
 *        this list of conditions and the following disclaimer.
 *  
 *     * Redistributions in binary form must reproduce the above copyright 
 *     notice,
 *        this list of conditions and the following disclaimer in the 
 *        documentation
 *        and/or other materials provided with the distribution.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 *  LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE 
 *  USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************************/



//#define DEBUG_TP
#define interesting 0x66df80
// #define interesting_era_l 5619814
// #define interesting_era_h 5620121

#ifndef CMDQUEUE_H
#define CMDQUEUE_H

//CommandQueue.h
//
//Header
//

#include "BusPacket.h"
#include "BankState.h"
#include "Transaction.h"
#include "SystemConfiguration.h"
#include "SimulatorObject.h"

using namespace std;

namespace DRAMSim
{
    class CommandQueue : public SimulatorObject
    {
        CommandQueue();
        public:
        ostream &dramsim_log;
        //typedefs
        typedef vector<BusPacket *> BusPacket1D;
        typedef vector<BusPacket1D> BusPacket2D;
        typedef vector<BusPacket2D> BusPacket3D;

        //functions
        CommandQueue(vector< vector<BankState> > &states, ostream &dramsim_log,
                int num_pids);
        virtual ~CommandQueue(); 

        virtual void enqueue(BusPacket *newBusPacket);
        virtual bool pop(BusPacket **busPacket);
        virtual bool hasRoomFor(unsigned numberToEnqueue, unsigned rank, unsigned 
                bank, unsigned pid);
        bool isIssuable(BusPacket *busPacket);
        virtual bool isEmpty(unsigned rank);
        void needRefresh(unsigned rank);
        virtual void print();
        void update(); //SimulatorObject requirement
        virtual vector<BusPacket *> &getCommandQueue(unsigned rank, unsigned bank);

        //fields

        BusPacket3D queues; // 3D array of BusPacket pointers
        vector< vector<BankState> > &bankStates;

        unsigned refreshCounter;

#ifdef DEBUG_TP
        virtual bool hasInteresting();
#endif /*DEBUG_TP*/

        protected:
        virtual void refreshPopClosePage(BusPacket **busPacket, bool & sendingREF);
        virtual bool normalPopClosePage(BusPacket **busPacket, bool & sendingREF);

        int num_pids;

        void nextRankAndBank(unsigned &rank, unsigned &bank);
        //fields
        unsigned nextBank;
        unsigned nextRank;
        unsigned nextThread;

        unsigned nextBankPRE;
        unsigned nextRankPRE;

        unsigned refreshRank;
        bool refreshWaiting;
        //flag to indicate the previous command is a row activation
        bool prev_ACTIVATE; 
        BusPacket *next_busPacket;

        vector< vector<unsigned> > tFAWCountdown;
        vector< vector<unsigned> > rowAccessCounters;
        vector<unsigned> threadCounters;

        bool sendAct;
    };
}

#endif

