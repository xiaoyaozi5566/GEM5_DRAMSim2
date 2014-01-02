#include "MemoryController.h"

using namespace std;

namespace DRAMSim
{
    class MemoryControllerTP : public MemoryController
    {
        public:
            MemoryControllerTP(MemorySystem* ms, CSVWriter &csvOut_, 
                    ostream &dramsim_log_, 
                    const string &outputFilename_,
                    unsigned tpTurnLength_,
		    bool genTrace_,
                    const string &traceFilename_,
                    int num_pids_, bool fixAddr,
                    bool diffPeriod, int p0Period, int p1Period);

            virtual bool addTransaction(Transaction *trans);
            virtual void receiveFromBus(BusPacket *bpacket);

        private:
            vector<Transaction *> transactionQueues[4];
            
            virtual void updateTransactionQueue();
            virtual void updateReturnTransactions();

            bool WillAcceptTransaction(uint64_t pid);
    };
}
