#include "CommandQueue.h"
#include "CommandQueueTP.h"
#include "MemoryController.h"

using namespace DRAMSim;

CommandQueueTP::CommandQueueTP(vector< vector<BankState> > &states, ostream 
        &dramsim_log_):
    CommandQueue(states,dramsim_log_)
{}

void CommandQueueTP::enqueue(BusPacket *newBusPacket)
{
    unsigned rank = newBusPacket->rank;
    unsigned pid = newBusPacket->threadID;
    queues[rank][pid].push_back(newBusPacket);
    if (queues[rank][pid].size()>CMD_QUEUE_DEPTH)
    {
        ERROR("== Error - Enqueued more than allowed in command queue");
        ERROR("						Need to call .hasRoomFor(int "
                "numberToEnqueue, unsigned rank, unsigned bank) first");
        exit(0);
    }
}

bool CommandQueueTP::hasRoomFor(unsigned numberToEnqueue, unsigned rank,
        unsigned bank, unsigned pid)
{
    vector<BusPacket *> &queue = getCommandQueue(rank, pid);
    return (CMD_QUEUE_DEPTH - queue.size() >= numberToEnqueue);
}

bool CommandQueueTP::isEmpty(unsigned rank)
{
    for(size_t i=0; i<NUM_PIDS; i++)
        if(!queues[rank][i].empty()) return false;
    return true;
}

vector<BusPacket *> &CommandQueueTP::getCommandQueue(unsigned rank, unsigned 
        pid)
{
    return queues[rank][pid];
}

void CommandQueueTP::refreshPopClosePage(BusPacket **busPacket, bool & 
        sendingREF)
{

    bool foundActiveOrTooEarly = false;
    //look for an open bank
    for (size_t b=0;b<NUM_BANKS;b++)
    {
        for (size_t pid_i=0; pid_i<NUM_PIDS; pid_i++){
            vector<BusPacket *> &queue = getCommandQueue(refreshRank,pid_i);
            //checks to make sure that all banks are idle
            if (bankStates[refreshRank][b].currentBankState == RowActive)
            {
                foundActiveOrTooEarly = true;
                //if the bank is open, make sure there is nothing else
                // going there before we close it
                for (size_t j=0;j<queue.size();j++)
                {
                    BusPacket *packet = queue[j];
                    if (packet->row == 
                            bankStates[refreshRank][b].openRowAddress &&
                            packet->bank == b)
                    {
                        if (packet->busPacketType != ACTIVATE && 
                                isIssuable(packet))
                        {
                            *busPacket = packet;
                            queue.erase(queue.begin() + j);
                            sendingREF = true;
                            cout << "Sending REF at time "
                                <<currentClockCycle<<endl;
                        }
                        break;
                    }
                }

                break;
            }
            //	NOTE: checks nextActivate time for each bank to make sure tRP 
            //	is being
            //				satisfied.	the next ACT and next REF can be issued 
            //				at the same
            //				point in the future, so just use nextActivate field 
            //				instead of
            //				creating a nextRefresh field
            else if (bankStates[refreshRank][b].nextActivate > 
                    currentClockCycle)
            {
                foundActiveOrTooEarly = true;
                break;
            }
        }
    }

    //if there are no open banks and timing has been met, send out the refresh
    //	reset flags and rank pointer
    if (!foundActiveOrTooEarly && bankStates[refreshRank][0].currentBankState 
            != PowerDown)
    {
        *busPacket = new BusPacket(REFRESH, 0, 0, 0, refreshRank, 0, 0, 
                dramsim_log);
        refreshRank = -1;
        refreshWaiting = false;
        sendingREF = true;
    }
}

bool CommandQueueTP::normalPopClosePage(BusPacket **busPacket, bool 
        &sendingREF)
{

    bool foundIssuable = false;
    unsigned startingRank = nextRank;
    unsigned startingBank = nextBank;
    unsigned currentPID = (currentClockCycle >> BLOCK_TIME) % (NUM_PIDS);
    if(lastPID!=currentPID){
        //if the turn changes, reset the nextRank, nextBank, and
        //starters. It seems to have no effect on interference.
        nextRank = nextBank =0;
        startingRank = nextRank;
        startingBank = nextBank;

    }
    lastPID = currentPID;
    unsigned btime = 1<<BLOCK_TIME;
    bool isBufferTime = (btime - (currentClockCycle & (btime-1))) -1  <= 151;

    while(true)
    {
        //Only get the queue for the PID with the current turn.
        vector<BusPacket *> &queue = getCommandQueue(nextRank, currentPID);
        //make sure there is something in this queue first
        //	also make sure a rank isn't waiting for a refresh
        //	if a rank is waiting for a refesh, don't issue anything to it until 
        //	the
        //		refresh logic above has sent one out (ie, letting banks close)
        if (!queue.empty() && !((nextRank == refreshRank) && refreshWaiting))
        {

            //search from beginning to find first issuable bus packet
            for (size_t i=0;i<queue.size();i++)
            {
                if (isIssuable(queue[i]))
                {
                    //If a turn change is about to happen, don't
                    //issue any activates

                    if(isBufferTime && queue[i]->busPacketType==ACTIVATE)
                        continue;

                    //check to make sure we aren't removing a read/write that 
                    //is paired with an activate
                    if (i>0 && queue[i-1]->busPacketType==ACTIVATE &&
                            queue[i-1]->physicalAddress == 
                            queue[i]->physicalAddress)
                        continue;

                    *busPacket = queue[i];
                    queue.erase(queue.begin()+i);
                    foundIssuable = true;
                    break;
                }
            }
        }

        //if we found something, break out of do-while
        if (foundIssuable) break;

        //rank round robin
        if (queuingStructure == PerRank)
        {
            nextRank = (nextRank + 1) % NUM_RANKS;
            if (startingRank == nextRank)
            {
                break;
            }
        }
        else {
            nextRankAndBank(nextRank, nextBank);
            if (startingRank == nextRank && startingBank == nextBank)
            {
                break;
            }
        }
    }

    //if we couldn't find anything to send, return false
    if (!foundIssuable) return false;
    return true;
}
