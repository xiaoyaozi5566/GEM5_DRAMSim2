#include "MemoryController.h"

using namespace std;

namespace DRAMSim
{
    class MemoryControllerFT : public MemoryController
    {
        public:
            MemoryControllerFT(MemorySystem* ms, CSVWriter &csvOut_, 
                    ostream &dramsim_log_, const string &outputFilename_, bool genTrace_,
       		    const string &traceFilename_, int num_pids, bool fixAddr);

            virtual bool addTransaction(Transaction *trans);
            virtual void receiveFromBus(BusPacket *bpacket);

        private:
            vector<Transaction *> transactionQueues[4];
            
            virtual void updateTransactionQueue();
            virtual void updateReturnTransactions();

            bool WillAcceptTransaction(uint64_t pid);
    };
}
