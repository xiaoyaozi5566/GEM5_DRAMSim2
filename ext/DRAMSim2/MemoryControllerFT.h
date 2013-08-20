#include "MemoryController.h"

using namespace std;

namespace DRAMSim
{
    class MemoryControllerFT : public MemoryController
    {
        public:
            MemoryControllerFT(MemorySystem* ms, CSVWriter &csvOut_, 
                    ostream &dramsim_log_, const string &outputFilename_,
                    int num_pids);

            virtual bool addTransaction(Transaction *trans);
            virtual void receiveFromBus(BusPacket *bpacket);

        private:
            vector<Transaction *> transactionQueues[4];
            
            virtual void updateTransactionQueue();
            virtual void updateReturnTransactions();

            bool WillAcceptTransaction(uint64_t pid);
    };
}
