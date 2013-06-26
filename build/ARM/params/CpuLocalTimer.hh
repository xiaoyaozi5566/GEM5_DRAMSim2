#ifndef __PARAMS__CpuLocalTimer__
#define __PARAMS__CpuLocalTimer__

class CpuLocalTimer;

#include <cstddef>
#include "params/Gic.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/BasicPioDevice.hh"

struct CpuLocalTimerParams
    : public BasicPioDeviceParams
{
    CpuLocalTimer * create();
    Gic * gic;
    uint32_t int_num_watchdog;
    uint32_t int_num_timer;
};

#endif // __PARAMS__CpuLocalTimer__
