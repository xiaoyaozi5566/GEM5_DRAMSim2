#ifndef __PARAMS__DRAMSim2Wrapper__
#define __PARAMS__DRAMSim2Wrapper__

class DRAMSim2Wrapper;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/AbstractMemory.hh"

struct DRAMSim2WrapperParams
    : public AbstractMemoryParams
{
    DRAMSim2Wrapper * create();
    Tick latency;
    Tick latency_var;
    unsigned int port_port_connection_count;
};

#endif // __PARAMS__DRAMSim2Wrapper__
