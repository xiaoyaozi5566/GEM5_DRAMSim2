#include "MemoryControllerTP.h"
#include "MemoryController.h"
#include "CommandQueueTP.h"
#include "AddressMapping.h"
using namespace DRAMSim;

MemoryControllerTP::MemoryControllerTP(MemorySystem *parent, 
        CSVWriter &csvOut_, ostream &dramsim_log_, 
        const string &outputFilename_,
        unsigned tpTurnLength,
        int num_pids_) :
    MemoryController(parent,csvOut_,dramsim_log_,outputFilename_, false, "",num_pids_)
{

    commandQueue = new CommandQueueTP(bankStates,dramsim_log_,tpTurnLength,num_pids_); 

    // reserve for each process
    for (int i=0;i<num_pids;i++){
        transactionQueues[i].reserve(TRANS_QUEUE_DEPTH);
    }
}


bool MemoryControllerTP::addTransaction(Transaction *trans)
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

void MemoryControllerTP::receiveFromBus(BusPacket *bpacket)
{
    //Change if needed later.
    MemoryController::receiveFromBus(bpacket);
}

void MemoryControllerTP::updateTransactionQueue()
{
    for (int j=0; j<num_pids; j++){
        for (size_t i=0;i<transactionQueues[j].size();i++)
        {
            // pop off top transaction from queue
            //
            //	assuming simple scheduling at the moment
            //	will eventually add policies here
            Transaction *transaction = transactionQueues[j][i];

            //map address to rank,bank,row,col
            unsigned newTransactionChan, newTransactionRank, 
                     newTransactionBank, newTransactionRow, 
                     newTransactionColumn;

            // pass these in as references so they get set by the 
            // addressMapping function
            addressMapping(transaction->address, newTransactionChan, 
                    newTransactionRank, newTransactionBank, newTransactionRow, 
                    newTransactionColumn);

            // if we have room, break up the transaction into the appropriate 
            // commands
            // and add them to the command queue
            if (commandQueue->hasRoomFor(2, newTransactionRank, 
                        newTransactionBank, j))
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



                //now that we know there is room in the command queue, we can 
                //remove from the transaction queue
                transactionQueues[j].erase(transactionQueues[j].begin()+i);

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



                //Formerly has second argument 'j'
                commandQueue->enqueue(ACTcommand);
                commandQueue->enqueue(command);

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
}

void MemoryControllerTP::updateReturnTransactions()
{
    MemoryController::updateReturnTransactions();
}

bool MemoryControllerTP::WillAcceptTransaction(uint64_t pid)
{

    return transactionQueues[pid].size() < TRANS_QUEUE_DEPTH;
}


/*
   void MemoryController::printtransactionQueues(){
   for (int j=0; j<num_pids; j++){
   PRINT("== Printing transaction queue" << j);
   for (size_t i=0;i<transactionQueues[j].size();i++)
   {
   PRINTN("  " << i << "] "<< *transactionQueues[j][i]);
   }
   }
   }
   */
