#include "CommandQueueFA.h"

using namespace DRAMSim;

CommandQueueFA::CommandQueueFA(vector< vector<BankState> > &states, 
        ostream &dramsim_log_, unsigned tpTurnLength_,
        int num_pids_, bool fixAddr_,
        bool diffPeriod_, int p0Period_, int p1Period_, int offset_) :
        CommandQueueTP(states,dramsim_log_,tpTurnLength_,num_pids_,fixAddr_,
          diffPeriod_, p0Period_, p1Period_, offset_)
{
    lastPID = 0;
    lastPID1 = 0;
}

bool CommandQueueFA::normalPopClosePage(BusPacket **busPacket, bool 
        &sendingREF)
{

    bool foundIssuable = false;
    unsigned startingRank = nextRank;
    unsigned startingBank = nextBank;
    if(lastPID!=getCurrentPID()){
        //if the turn changes, reset the nextRank, nextBank, and
        //starters. It seems to have no effect on interference.
        nextRank = nextBank =0;
        startingRank = nextRank;
        startingBank = nextBank;
#ifdef DEBUG_TP
        cout << endl << "==========================================="<<endl;
        cout << "Starting turn of length 2**"<<tpTurnLength<<" with PID "<<
            getCurrentPID() <<" at cycle "<< currentClockCycle << endl;
        cout << endl;
        print();
#endif /*DEBUG_TP*/
        // for fixed address mapping
        lastPID1 = lastPID;
    }
    lastPID = getCurrentPID();

    while(true)
    {
        //Only get the queue for the PID with the current turn.
        vector<BusPacket *> &queue = getCommandQueue(nextRank, getCurrentPID());
        vector<BusPacket *> &queue1 = getCommandQueue(nextRank, lastPID1);
        //make sure there is something in this queue first
        //	also make sure a rank isn't waiting for a refresh
        //	if a rank is waiting for a refesh, don't issue anything to it until 
        //	the
        //		refresh logic above has sent one out (ie, letting banks close)

#ifdef DEBUG_TP
        if(hasInteresting()){
            printf("nextRank %u refreshRank %u currentPID %u\n",nextRank,refreshRank,getCurrentPID());
        }
#endif /*DEBUG_TP*/

        if (!((nextRank == refreshRank) && refreshWaiting))
        {

            //search from beginning to find first issuable bus packet
            for (size_t i=0;i<queue1.size();i++)
            {

                if (isIssuable(queue1[i]))
                {
#ifdef DEBUG_TP
                    if(lastPopTime!=currentClockCycle &&
                            queue1[i]->physicalAddress == interesting){
                        string bptype = (queue1[i]->busPacketType==ACTIVATE) ?
                            "activate" : "r/w";
                        cout << "popped interesting "<< bptype << " @ "
                            << currentClockCycle << endl;
                        lastPopTime = currentClockCycle;
                    }
#endif /*DEBUG_TP*/
                    //If a turn change is about to happen, don't
                    //issue any activates

                    if(queue1[i]->busPacketType==ACTIVATE)
                        continue;

                    //check to make sure we aren't removing a read/write that 
                    //is paired with an activate
                    // if (i>0 && queue[i-1]->busPacketType==ACTIVATE &&
//                             queue[i-1]->physicalAddress == 
//                             queue[i]->physicalAddress)
//                         continue;

                    *busPacket = queue1[i];

                    queue1.erase(queue1.begin()+i);
                    foundIssuable = true;
                    break;
                }
#ifdef DEBUG_TP
                else if(queue1[i]->physicalAddress==interesting)
                {
                    string bptype = (queue1[i]->busPacketType==ACTIVATE) ?
                        "activate" : "r/w";
                    cout << "interesting couldn't issue @ "<<
                        currentClockCycle << " as a "<<bptype <<endl;
                    cout << "nextRank "<<nextRank<< " nextBank "<<nextBank
                        << endl << "startingRank "<<startingRank
                        <<" startingBank " << startingBank << endl;
                    printf("refreshRank %u\n",refreshRank);
                    bankStates[queue1[i]->rank][queue1[i]->bank].print();
                    //lastPopTime = currentClockCycle;
                }
#endif /*DEBUG_TP*/
            }
            
            if ( foundIssuable == false ) {
            //search from beginning to find first issuable bus packet
            for (size_t i=0;i<queue.size();i++)
            {

                if (isIssuable(queue[i]))
                {
#ifdef DEBUG_TP
                    if(lastPopTime!=currentClockCycle &&
                            queue[i]->physicalAddress == interesting){
                        string bptype = (queue[i]->busPacketType==ACTIVATE) ?
                            "activate" : "r/w";
                        cout << "popped interesting "<< bptype << " @ "
                            << currentClockCycle << endl;
                        lastPopTime = currentClockCycle;
                    }
#endif /*DEBUG_TP*/
                    //If a turn change is about to happen, don't
                    //issue any activates

                    if(isBufferTime() && queue[i]->busPacketType==ACTIVATE)
                        continue;

                    //check to make sure we aren't removing a read/write that 
                    //is paired with an activate
                    // if (i>0 && queue[i-1]->busPacketType==ACTIVATE &&
//                             queue[i-1]->physicalAddress == 
//                             queue[i]->physicalAddress)
//                         continue;

                    *busPacket = queue[i];

                    queue.erase(queue.begin()+i);
                    foundIssuable = true;
                    break;
                }
#ifdef DEBUG_TP
                else if(queue[i]->physicalAddress==interesting)
                {
                    string bptype = (queue[i]->busPacketType==ACTIVATE) ?
                        "activate" : "r/w";
                    cout << "interesting couldn't issue @ "<<
                        currentClockCycle << " as a "<<bptype <<endl;
                    cout << "nextRank "<<nextRank<< " nextBank "<<nextBank
                        << endl << "startingRank "<<startingRank
                        <<" startingBank " << startingBank << endl;
                    printf("refreshRank %u\n",refreshRank);
                    bankStates[queue[i]->rank][queue[i]->bank].print();
                    //lastPopTime = currentClockCycle;
                }
#endif /*DEBUG_TP*/
            }
            }
        }
#ifdef DEBUG_TP 
        else if(hasInteresting() && refreshWaiting && nextRank ==refreshRank){
            //  PRINTN("Blocked by refreshRank at "<<currentClockCycle<<
            //         " with turn "<<currentPID<<endl);
        }
#endif /*DEBUG_TP*/

        //if we found something, break out of do-while
        if (foundIssuable) break;

        nextRankAndBank(nextRank, nextBank);
        if (startingRank == nextRank && startingBank == nextBank)
        {
            break;
        }
    }

    return foundIssuable;
}
