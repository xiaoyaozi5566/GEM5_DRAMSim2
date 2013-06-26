#ifndef __PARAMS__AtomicSimpleCPU__
#define __PARAMS__AtomicSimpleCPU__

class AtomicSimpleCPU;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <cstddef>

#include "params/BaseSimpleCPU.hh"

struct AtomicSimpleCPUParams
    : public BaseSimpleCPUParams
{
    AtomicSimpleCPU * create();
    int width;
    bool simulate_data_stalls;
    bool fastmem;
    bool simulate_inst_stalls;
};

#endif // __PARAMS__AtomicSimpleCPU__
