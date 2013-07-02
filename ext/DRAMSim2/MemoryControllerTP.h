#include "MemoryController.h"

using namespace std;

namespace DRAMSim
{
    class MemoryControllerTP : public MemoryController
    {
        public:
            MemoryControllerTP(MemorySystem* ms, CSVWriter &csvOut_, 
                    ostream &dramsim_log_, const string &outputFilename_);

            virtual bool addTransaction(Transaction *trans);
            virtual void receiveFromBus(BusPacket *bpacket);

        private:
            vector<Transaction *> transactionQueues[NUM_PIDS];
            
            virtual void updateTransactionQueue();
            virtual void updateReturnTransactions();

            bool WillAcceptTransaction(uint64_t pid);
    };
}
