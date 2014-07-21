#include "CommandQueueFA.h"

#define BLOCK_TIME 12
// #define DEBUG_TP

using namespace std;

namespace DRAMSim
{
    class CommandQueueTPD : public CommandQueueTP
    {
        public:
            CommandQueueTPD(vector< vector<BankState> > &states,
                    ostream &dramsim_log_,unsigned tpTurnLength,
                    int num_pids, bool fixAddr_,
                    bool diffPeriod_, int p0Period_, int p1Period_, int offset_);

        protected:
            virtual bool normalPopClosePage(BusPacket **busPacket, bool & sendingREF);

            virtual int normal_deadtime(int tlength){
			  return WORST_CASE_DELAY;
            }

            virtual int refresh_deadtime(int tlength){
			  return TP_BUFFER_TIME;
            }
    };
}
