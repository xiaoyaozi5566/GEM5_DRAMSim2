#include "CommandQueueTP.h"

using namespace std;

namespace DRAMSim
{
    class CommandQueueFA : public CommandQueueTP
    {
        public:
            CommandQueueFA(vector< vector<BankState> > &states,
                    ostream &dramsim_log_,unsigned tpTurnLength,
                    int num_pids, bool fixAddr_,
                    bool diffPeriod_, int p0Period_,
                    int p1Period_, int offset_);
        protected:
            virtual bool normalPopClosePage(BusPacket **busPacket, bool & sendingREF);

            unsigned lastPID;
            unsigned lastPID1;

            virtual int normal_deadtime(int tlength){
              return tlength - (tlength - FIX_WORST_CASE_DELAY)/10;
            }

            virtual int refresh_deadtime(int tlength){
              return tlength - (tlength - FIX_TP_BUFFER_TIME)/10;
            }
    };
}
