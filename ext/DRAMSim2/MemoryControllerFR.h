#include "MemoryController.h"

using namespace std;

namespace DRAMSim
{
    class MemoryControllerFR : public MemoryController
    {
        public:
            MemoryControllerFR(MemorySystem* ms, CSVWriter &csvOut_, ostream 
                    &dramsim_log_, const string &outputFilename_, bool genTrace_,
                    const string &traceFilename_, int num_pids_, bool fixAddr);
    };
}
