#ifndef __PARAMS__DRAMSim2__
#define __PARAMS__DRAMSim2__

class DRAMSim2;

#include <cstddef>
#include <string>
#include <cstddef>
#include <string>
#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <string>

#include "params/DRAMSim2Wrapper.hh"

struct DRAMSim2Params
    : public DRAMSim2WrapperParams
{
    DRAMSim2 * create();
    std::string deviceConfigFile;
    std::string traceFile;
    std::string systemConfigFile;
    Tick cpu_clock;
    bool finalStatsOnly;
    std::string cwd;
};

#endif // __PARAMS__DRAMSim2__
