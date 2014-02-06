#include "CommandQueue.h"
#include "CommandQueueFR.h"
#include "MemoryController.h"

using namespace DRAMSim;

CommandQueueFR::CommandQueueFR(vector< vector<BankState> > &states, 
        ostream &dramsim_log_,int num_pids_):
    CommandQueue(states,dramsim_log_,num_pids_)
{
}

bool CommandQueueFR::pop(BusPacket **busPacket)
{
    //this can be done here because pop() is called every clock cycle by the parent MemoryController
    //	figures out the sliding window requirement for tFAW
    //
    //deal with tFAW book-keeping
    //	each rank has it's own counter since the restriction is on a device level
    for (size_t i=0;i<NUM_RANKS;i++)
    {
        //decrement all the counters we have going
        for (size_t j=0;j<tFAWCountdown[i].size();j++)
        {
            tFAWCountdown[i][j]--;
        }

        //the head will always be the smallest counter, so check if it has reached 0
        if (tFAWCountdown[i].size()>0 && tFAWCountdown[i][0]==0)
        {
            tFAWCountdown[i].erase(tFAWCountdown[i].begin());
        }
    }

    /* Now we need to find a packet to issue. When the code picks a packet, it will set
     *busPacket = [some eligible packet]

     First the code looks if any refreshes need to go
     Then it looks for data packets
     Otherwise, it starts looking for rows to close (in open page)
     */

    if (rowBufferPolicy==ClosePage)
    {
        bool sendingREF = false;
        //if the memory controller set the flags signaling that we need to issue a refresh
        if (refreshWaiting)
        {
            refreshPopClosePage(busPacket,sendingREF);

        } // refreshWaiting

        //if we're not sending a REF, proceed as normal
        if (!sendingREF)
        {
            if (!normalPopClosePage(busPacket,sendingREF))
                return false;
        }
    }
    else if (rowBufferPolicy==OpenPage)
    {
        bool sendingREForPRE = false;
        if (refreshWaiting)
        {
            bool sendREF = true;
            //make sure all banks idle and timing met for a REF
            for (size_t b=0;b<NUM_BANKS;b++)
            {
                //if a bank is active we can't send a REF yet
                if (bankStates[refreshRank][b].currentBankState == RowActive)
                {
                    sendREF = false;
                    bool closeRow = true;
                    //search for commands going to an open row
                    vector <BusPacket *> &refreshQueue = getCommandQueue(refreshRank,b);

                    for (size_t j=0;j<refreshQueue.size();j++)
                    {
                        BusPacket *packet = refreshQueue[j];
                        //if a command in the queue is going to the same row . . .
                        if (bankStates[refreshRank][b].openRowAddress == packet->row &&
                                b == packet->bank)
                        {
                            // . . . and is not an activate . . .
                            if (packet->busPacketType != ACTIVATE)
                            {
                                closeRow = false;
                                // . . . and can be issued . . .
                                if (isIssuable(packet))
                                {
                                    //send it out
                                    *busPacket = packet;
                                    refreshQueue.erase(refreshQueue.begin()+j);
                                    sendingREForPRE = true;
                                }
                                break;
                            }
                            else //command is an activate
                            {
                                //if we've encountered another act, no other command will be of interest
                                break;
                            }
                        }
                    }

                    //if the bank is open and we are allowed to close it, then send a PRE
                    if (closeRow && currentClockCycle >= bankStates[refreshRank][b].nextPrecharge)
                    {
                        rowAccessCounters[refreshRank][b]=0;
                        *busPacket = new BusPacket(PRECHARGE, 0, 0, 0, refreshRank, b, 0, dramsim_log);
                        sendingREForPRE = true;
                    }
                    break;
                }
                //	NOTE: the next ACT and next REF can be issued at the same
                //				point in the future, so just use nextActivate field instead of
                //				creating a nextRefresh field
                else if (bankStates[refreshRank][b].nextActivate > currentClockCycle) //and this bank doesn't have an open row
                {
                    sendREF = false;
                    break;
                }
            }

            //if there are no open banks and timing has been met, send out the refresh
            //	reset flags and rank pointer
            if (sendREF && bankStates[refreshRank][0].currentBankState != PowerDown)
            {
                *busPacket = new BusPacket(REFRESH, 0, 0, 0, refreshRank, 0, 0, dramsim_log);
                refreshRank = -1;
                refreshWaiting = false;
                sendingREForPRE = true;
            }
        }

        if (!sendingREForPRE)
        {
            unsigned startingRank = nextRank;
            unsigned startingBank = nextBank;
            bool foundIssuable = false;
            do // round robin over queues
            {
                vector<BusPacket *> &queue = getCommandQueue(nextRank,nextBank);
                // prioritize access to open row first
                if (!queue.empty() && !((nextRank == refreshRank) && refreshWaiting))
                {
                    //search from the beginning to find first issuable bus packet
                    for (size_t i=0;i<queue.size();i++)
                    {
                        BusPacket *packet = queue[i];
                        if (isIssuable(packet))
                        {
                            //check for dependencies
                            bool dependencyFound = false;
                            for (size_t j=0;j<i;j++)
                            {
                                BusPacket *prevPacket = queue[j];
                                if (prevPacket->busPacketType != ACTIVATE &&
                                        prevPacket->bank == packet->bank &&
                                        prevPacket->row == packet->row)
                                {
                                    dependencyFound = true;
                                    break;
                                }
                            }
                            if (dependencyFound) continue;

                            *busPacket = packet;

                            //if the bus packet before is an activate, that is the act that was
                            //	paired with the column access we are removing, so we have to remove
                            //	that activate as well (check i>0 because if i==0 then theres nothing before it)
                            if (i>0 && queue[i-1]->busPacketType == ACTIVATE)
                            {
                                rowAccessCounters[(*busPacket)->rank][(*busPacket)->bank]++;
                                // i is being returned, but i-1 is being thrown away, so must delete it here 
                                delete (queue[i-1]);

                                // remove both i-1 (the activate) and i (we've saved the pointer in *busPacket)
                                queue.erase(queue.begin()+i-1,queue.begin()+i+1);
                                foundIssuable = true;
                                break;
                            }
                        }
                    }
                }

                //if we found something, break out of do-while
                if (foundIssuable) break;
                
                //=============================================================
                //make sure there is something there first
                if (!queue.empty() && !((nextRank == refreshRank) && refreshWaiting))
                {
                    //search from the beginning to find first issuable bus packet
                    for (size_t i=0;i<queue.size();i++)
                    {
                        BusPacket *packet = queue[i];
                        if (isIssuable(packet))
                        {
                            //check for dependencies
                            bool dependencyFound = false;
                            for (size_t j=0;j<i;j++)
                            {
                                BusPacket *prevPacket = queue[j];
                                if (prevPacket->busPacketType != ACTIVATE &&
                                        prevPacket->bank == packet->bank &&
                                        prevPacket->row == packet->row)
                                {
                                    dependencyFound = true;
                                    break;
                                }
                            }
                            if (dependencyFound) continue;

                            *busPacket = packet;

                            //if the bus packet before is an activate, that is the act that was
                            //	paired with the column access we are removing, so we have to remove
                            //	that activate as well (check i>0 because if i==0 then theres nothing before it)
                            if (i>0 && queue[i-1]->busPacketType == ACTIVATE)
                            {
                                rowAccessCounters[(*busPacket)->rank][(*busPacket)->bank]++;
                                // i is being returned, but i-1 is being thrown away, so must delete it here 
                                delete (queue[i-1]);

                                // remove both i-1 (the activate) and i (we've saved the pointer in *busPacket)
                                queue.erase(queue.begin()+i-1,queue.begin()+i+1);
                            }
                            else // there's no activate before this packet
                            {
                                //or just remove the one bus packet
                                queue.erase(queue.begin()+i);
                            }

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
                else 
                {
                    nextRankAndBank(nextRank, nextBank); 
                    if (startingRank == nextRank && startingBank == nextBank)
                    {
                        break;
                    }
                }
            }
            while (true);

            //if nothing was issuable, see if we can issue a PRE to an open bank
            //	that has no other commands waiting
            if (!foundIssuable)
            {
                //search for banks to close
                bool sendingPRE = false;
                unsigned startingRank = nextRankPRE;
                unsigned startingBank = nextBankPRE;

                do // round robin over all ranks and banks
                {
                    vector <BusPacket *> &queue = getCommandQueue(nextRankPRE, nextBankPRE);
                    bool found = false;
                    //check if bank is open
                    if (bankStates[nextRankPRE][nextBankPRE].currentBankState == RowActive)
                    {
                        for (size_t i=0;i<queue.size();i++)
                        {
                            //if there is something going to that bank and row, then we don't want to send a PRE
                            if (queue[i]->bank == nextBankPRE &&
                                    queue[i]->row == bankStates[nextRankPRE][nextBankPRE].openRowAddress)
                            {
                                found = true;
                                break;
                            }
                        }

                        //if nothing found going to that bank and row or too many accesses have happend, close it
                        if (!found || rowAccessCounters[nextRankPRE][nextBankPRE]==TOTAL_ROW_ACCESSES)
                        {
                            if (currentClockCycle >= bankStates[nextRankPRE][nextBankPRE].nextPrecharge)
                            {
                                sendingPRE = true;
                                rowAccessCounters[nextRankPRE][nextBankPRE] = 0;
                                *busPacket = new BusPacket(PRECHARGE, 0, 0, 0, nextRankPRE, nextBankPRE, 0, dramsim_log);
                                break;
                            }
                        }
                    }
                    nextRankAndBank(nextRankPRE, nextBankPRE);
                }
                while (!(startingRank == nextRankPRE && startingBank == nextBankPRE));

                //if no PREs could be sent, just return false
                if (!sendingPRE) return false;
            }
        }
    }

    //sendAct is flag used for posted-cas
    //  posted-cas is enabled when AL>0
    //  when sendAct is true, when don't want to increment our indexes
    //  so we send the column access that is paid with this act
    if (AL>0 && sendAct)
    {
        sendAct = false;
    }
    else
    {
        sendAct = true;
        nextRankAndBank(nextRank, nextBank);
    }

    //if its an activate, add a tfaw counter
    if ((*busPacket)->busPacketType==ACTIVATE)
    {
        tFAWCountdown[(*busPacket)->rank].push_back(tFAW);
    }

    return true;
}

bool CommandQueueFR::normalPopClosePage(BusPacket **busPacket, bool & sendingREF)
{
    bool foundIssuable = false;
    unsigned startingRank = nextRank;
    unsigned startingBank = nextBank;

    do
    {
        vector<BusPacket *> &queue = getCommandQueue(nextRank, nextBank);
        //make sure there is something in this queue first
        //	also make sure a rank isn't waiting for a refresh
        //	if a rank is waiting for a refesh, don't issue anything to it until the
        //		refresh logic above has sent one out (ie, letting banks close)
        if (!queue.empty() && !((nextRank == refreshRank) && refreshWaiting))
        {
            if (queuingStructure == PerRank)
            {

                //search from beginning to find first issuable bus packet
                for (size_t i=0;i<queue.size();i++)
                {
                    if (isIssuable(queue[i]))
                    {
                        //check to make sure we aren't removing a read/write that is paired with an activate
                        if (i>0 && queue[i-1]->busPacketType==ACTIVATE &&
                                queue[i-1]->physicalAddress == queue[i]->physicalAddress)
                            continue;

                        *busPacket = queue[i];
                        queue.erase(queue.begin()+i);
                        foundIssuable = true;
                        break;
                    }
                }
            }
            else
            {
                for (size_t i=0;i<queue.size();i++)
				{
					BusPacket *packet = queue[i];
					if (isIssuable(packet))
					{
						//check for dependencies
						bool dependencyFound = false;
						for (size_t j=0;j<i;j++)
						{
							BusPacket *prevPacket = queue[j];
							if (prevPacket->busPacketType != ACTIVATE &&
									prevPacket->bank == packet->bank &&
									prevPacket->row == packet->row)
							{
								dependencyFound = true;
								break;
							}
						}
						if (dependencyFound) continue;

						*busPacket = packet;

						//if the bus packet before is an activate, that is the act that was
						//	paired with the column access we are removing, so we have to remove
						//	that activate as well (check i>0 because if i==0 then theres nothing before it)
						if (i>0 && queue[i-1]->busPacketType == ACTIVATE)
						{
							rowAccessCounters[(*busPacket)->rank][(*busPacket)->bank]++;
							// i is being returned, but i-1 is being thrown away, so must delete it here 
							delete (queue[i-1]);

							// remove both i-1 (the activate) and i (we've saved the pointer in *busPacket)
							queue.erase(queue.begin()+i-1,queue.begin()+i+1);
							foundIssuable = true;
							break;
						}
					}
				}
				if(!foundIssuable)
				{
					if (isIssuable(queue[0]))
					{

						//no need to search because if the front can't be sent,
						// then no chance something behind it can go instead
						*busPacket = queue[0];
						queue.erase(queue.begin());
						foundIssuable = true;
					}
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
        else 
        {
            nextRankAndBank(nextRank, nextBank);
            if (startingRank == nextRank && startingBank == nextBank)
            {
                break;
            }
        }
    }
    while (true);

    //if we couldn't find anything to send, return false
    if (!foundIssuable) return false;
    return true;
}