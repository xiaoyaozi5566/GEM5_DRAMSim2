#include "CommandQueue.h"
#include "CommandQueueFT.h"
#include "MemoryController.h"

using namespace DRAMSim;

CommandQueueFT::CommandQueueFT(vector< vector<BankState> > &states, ostream 
        &dramsim_log_):
    CommandQueue(states,dramsim_log_)
{
    //TODO
}

void CommandQueueFT::enqueue(BusPacket *newBusPacket)
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

bool CommandQueueFT::hasRoomFor(unsigned numberToEnqueue, unsigned rank,
        unsigned bank, unsigned pid)
{
    vector<BusPacket *> &queue = getCommandQueue(rank, pid);
    return (CMD_QUEUE_DEPTH - queue.size() >= numberToEnqueue);
}

bool CommandQueueFT::isEmpty(unsigned rank)
{
    for (size_t i=0;i<NUM_PIDS;i++)
    {
        if (!queues[rank][i].empty()) return false;
    }
    return true;
}

vector<BusPacket *> &CommandQueueFT::getCommandQueue(unsigned rank, 
        unsigned pid)
{
    return queues[rank][pid];
}

void CommandQueueFT::refreshPopClosePage(BusPacket **busPacket, 
        bool & sendingREF)
{
    bool foundActiveOrTooEarly = false;
    //look for an open bank
    for (size_t b=0;b<NUM_BANKS;b++)
    {
        if (bankStates[refreshRank][b].currentBankState == RowActive)
        {
            foundActiveOrTooEarly = true;
            for (size_t p=0;p<NUM_PIDS;p++)
            {
                vector<BusPacket *> &queue = getCommandQueue(refreshRank, p);
                BusPacket *packet = queue[0];
                if (packet->row == bankStates[refreshRank][b].openRowAddress &&
                        packet->bank == b)
                {
                    if (packet->busPacketType != ACTIVATE && 
                            isIssuable(packet))
                    {
                        *busPacket = packet;
                        queue.erase(queue.begin());
                        sendingREF = true;
                    }
                    break;
                }
            }
            break;
        }
        //	NOTE: checks nextActivate time for each bank to make sure tRP is 
        //	being
        //				satisfied.	the next ACT and next REF can be issued at 
        //				the same
        //				point in the future, so just use nextActivate field 
        //				instead of
        //				creating a nextRefresh field
        else if (bankStates[refreshRank][b].nextActivate > currentClockCycle)
        {
            foundActiveOrTooEarly = true;
            break;
        }
    }

    //if there are no open banks and timing has been met, send out the refresh
    //	reset flags and rank pointer
    if (!foundActiveOrTooEarly && bankStates[refreshRank][0].currentBankState != PowerDown)
    {
        *busPacket = new BusPacket(REFRESH, 0, 0, 0, refreshRank, 0, 0, dramsim_log);
        PRINTN("Refresh at " << currentClockCycle << " for rank " << refreshRank << endl);
        refreshRank = -1;
        refreshWaiting = false;
        sendingREF = true;
        refreshCounter++;
    }
}

bool CommandQueueFT::normalPopClosePage(BusPacket **busPacket, 
        bool &sendingREF)
{
    bool foundIssuable = false;
    unsigned worstStartTime = (threadCounters[nextThread]*NUM_PIDS + 
            nextThread)*WORST_CASE_DELAY + tRFC*refreshCounter;
    if (prev_ACTIVATE)
    {
        if (isIssuable(next_busPacket))
        {
            *busPacket = next_busPacket;
            //(*busPacket)->index = threadCounters[nextThread];
            (*busPacket)->returnTime = worstStartTime + WORST_CASE_DELAY;
            foundIssuable = true;
            prev_ACTIVATE = false;
        }
    }
    else
    {
        if(currentClockCycle == worstStartTime)
        {
            for (size_t r=0;r<NUM_RANKS;r++)
            {
                vector<BusPacket *> &queue = getCommandQueue(r, nextThread);
                if (!queue.empty() && !((r == refreshRank) && refreshWaiting))
                {
                    for (size_t i=0;i<queue.size();i++)
                    {
                        if (isIssuable(queue[i]) && queue[i]->busPacketType == 
                                ACTIVATE)
                        {							
                            *busPacket = queue[i];
                            next_busPacket = queue[i+1];
                            queue.erase(queue.begin()+i);
                            queue.erase(queue.begin()+i);
                            foundIssuable = true;
                            prev_ACTIVATE = true;
                            break;
                        }
                    }
                }
                else if (!queue.empty())
                {
                    break;
                }
                if (foundIssuable) break;
            }
        }
    }
    if (!foundIssuable && (currentClockCycle < worstStartTime)) return false;
    // can't find issuable command even after the worst case time (possibly 
    // empty command queue)
    else if (!foundIssuable && (currentClockCycle == worstStartTime))
    {
        threadCounters[nextThread]++;
        nextThread = (nextThread+1)%NUM_PIDS;
        return false;
    }				else if (foundIssuable)
    {
        if((*busPacket)->busPacketType!=ACTIVATE)
        {
            threadCounters[nextThread]++;
            nextThread = (nextThread+1)%NUM_PIDS;
        }
    }
    else
    {
        return false;
    }
    return true;
}
