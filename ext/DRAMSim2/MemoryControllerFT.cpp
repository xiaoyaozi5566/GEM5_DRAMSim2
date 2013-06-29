#include "MemoryControllerFT.h"
#include "MemoryController.h"
#include "AddressMapping.h"
using namespace DRAMSim;

MemoryControllerFT::MemoryControllerFT(MemorySystem *parent, CSVWriter 
        &csvOut_, ostream &dramsim_log_, const string &outputFilename_) :
    MemoryController(parent,csvOut_,dramsim_log_,outputFilename_)
{
    // reserve for each process
    for (int i=0;i<NUM_PIDS;i++){
        transactionQueues[i].reserve(TRANS_QUEUE_DEPTH);
    }
}

bool MemoryControllerFT::WillAcceptTransaction(uint64_t pid)
{
    return transactionQueues[pid].size() < TRANS_QUEUE_DEPTH;
}

void MemoryControllerFT::updateTransactionQueue()
{
    for (size_t i=0;i<NUM_PIDS;i++)
        for (size_t j=0;j<transactionQueues[i].size();j++)
        {
            Transaction *transaction = transactionQueues[i][j];
            //map address to rank,bank,row,col
            unsigned newTransactionChan, newTransactionRank, 
                     newTransactionBank, newTransactionRow, 
                     newTransactionColumn;

            // pass these in as references so they get set by the 
            // addressMapping function
            addressMapping(transaction->address, newTransactionChan, 
                    newTransactionRank, newTransactionBank, newTransactionRow, 
                    newTransactionColumn);

            //if we have room, break up the transaction into the appropriate 
            //commands
            //and add them to the command queue
            if (commandQueue.hasRoomFor(2, newTransactionRank, 
                        newTransactionBank, transaction->threadID))
            {
                if (DEBUG_ADDR_MAP) {
                    PRINTN("== New Transaction - Mapping Address [0x" << hex << 
                            transaction->address << dec << "]");
                    if (transaction->transactionType == DATA_READ) {
                        PRINT(" (Read)");
                    }
                    else
                    {
                        PRINT(" (Write)");
                    }
                    PRINT("  Rank : " << newTransactionRank);
                    PRINT("  Bank : " << newTransactionBank);
                    PRINT("  Row  : " << newTransactionRow);
                    PRINT("  Col  : " << newTransactionColumn);
                }
                //PRINT("ID: " << i << " Request " << hex << 
                //transaction->address << " enqueue at " << dec << 
                //currentClockCycle);



                //now that we know there is room in the command queue, we can 
                //remove from the transaction queue
                transactionQueues[i].erase(transactionQueues[i].begin()+j);

                //create activate command to the row we just translated
                BusPacket *ACTcommand = new BusPacket(ACTIVATE, 
                        transaction->address,
                        newTransactionColumn, newTransactionRow, 
                        newTransactionRank,
                        newTransactionBank, 0, transaction->threadID, 
                        dramsim_log);

                //create read or write command and enqueue it
                BusPacketType bpType = transaction->getBusPacketType();
                BusPacket *command = new BusPacket(bpType, 
                        transaction->address,
                        newTransactionColumn, newTransactionRow, 
                        newTransactionRank,
                        newTransactionBank, transaction->data, 
                        transaction->threadID, dramsim_log);



                commandQueue.enqueue(ACTcommand);
                commandQueue.enqueue(command);

                // If we have a read, save the transaction so when the data 
                // comes back
                // in a bus packet, we can staple it back into a transaction 
                // and return it
                if (transaction->transactionType == DATA_READ)
                {
                    pendingReadTransactions.push_back(transaction);
                }
                else
                {
                    // just delete the transaction now that it's a buspacket
                    delete transaction; }
                /* only allow one transaction to be scheduled per cycle -- this 
                 * should
                 * be a reasonable assumption considering how much logic would 
                 * be
                 * required to schedule multiple entries per cycle (parallel 
                 * data
                 * lines, switching logic, decision logic)
                 */
                break;
            }
            else // no room, do nothing this cycle
            {
                //PRINT( "== Warning - No room in command queue" << endl;
            }
        }
}

void MemoryControllerFT::updateReturnTransactions()
{
    if (returnTransaction.size()>0)
    {
        if (DEBUG_BUS)
        {
            PRINTN(" -- MC Issuing to CPU bus : " << *returnTransaction[0]);
        }
        totalTransactions++;

        bool foundMatch=false;
        size_t j;
        //find the pending read transaction to calculate latency
        for (j=0;j<returnTransaction.size();j++)
        {
            unsigned returnTime = returnTransaction[j]->returnTime;
            // Spin until the proper return time has been reached.
            if (returnTime > currentClockCycle) continue;
            else
            {
                for (size_t i=0;i<pendingReadTransactions.size();i++)
                {
                    if (pendingReadTransactions[i]->address == 
                            returnTransaction[j]->address)
                    {
                        unsigned chan,rank,bank,row,col;
                        addressMapping(returnTransaction[j]->address,chan,rank,bank,row,col);
                        returnReadData(pendingReadTransactions[i]);
                        if (returnTransaction[j]->threadID == 0){
                            outputFile << "Address: " << hex << 
                                returnTransaction[j]->address << "  Return "
                                "time: " << dec << currentClockCycle << '\n';
                            cout << "Address: " << hex << 
                                returnTransaction[j]->address << "  Return "
                                "time: " << dec << currentClockCycle << '\n';
                        }

                        delete pendingReadTransactions[i];
                        pendingReadTransactions.erase(pendingReadTransactions.begin()+i);
                        foundMatch=true; delete returnTransaction[j];
                        returnTransaction.erase(returnTransaction.begin()+j);
                        break;
                    }
                }
                if (!foundMatch)
                {
                    ERROR("Can't find a matching transaction for "
                            "  0x"<<hex<<returnTransaction[j]->address<<dec);
                    abort(); }
                break;
            }
        }
    }
}

void MemoryControllerFT::receiveFromBus(BusPacket *bpacket)
{
    //Change this later if needed
    MemoryController::receiveFromBus(bpacket);
}

bool MemoryControllerFT::addTransaction(Transaction *trans)
{
    if (WillAcceptTransaction(trans->threadID))
    {
        transactionQueues[trans->threadID].push_back(trans);
        return true;
    }
    else {
        return false;
    }
}
