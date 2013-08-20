#include "CommandQueue.h"

using namespace std;

namespace DRAMSim
{
    class CommandQueueFT : public CommandQueue
    {
        public:
            CommandQueueFT(vector< vector<BankState> > &states,
                    ostream &dramsim_log_, int num_pids);
            virtual void enqueue(BusPacket *newBusPacket);
            virtual bool hasRoomFor(unsigned numberToEnqueue, unsigned rank, 
                    unsigned bank, unsigned pid);
            virtual bool isEmpty(unsigned rank);
            virtual vector<BusPacket *> &getCommandQueue(unsigned rank, 
                    unsigned pid);

        private:
            virtual void refreshPopClosePage(BusPacket **busPacket, bool & sendingREF);
            virtual bool normalPopClosePage(BusPacket **busPacket, bool & sendingREF);
    };
}
