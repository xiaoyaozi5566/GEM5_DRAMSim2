#include "CommandQueue.h"

#define BLOCK_TIME 12

using namespace std;

namespace DRAMSim
{
    class CommandQueueFR : public CommandQueue
    {
        public:
            CommandQueueFR(vector< vector<BankState> > &states, ostream &dramsim_log,
                int num_pids);
        private:
        	virtual bool pop(BusPacket **busPacket);
        	virtual bool normalPopClosePage(BusPacket **busPacket, bool & sendingREF);
    };
}