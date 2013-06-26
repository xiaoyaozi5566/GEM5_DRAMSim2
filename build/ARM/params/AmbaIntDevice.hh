#ifndef __PARAMS__AmbaIntDevice__
#define __PARAMS__AmbaIntDevice__

class AmbaIntDevice;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/Gic.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/AmbaDevice.hh"

struct AmbaIntDeviceParams
    : public AmbaDeviceParams
{
    uint32_t int_num;
    Gic * gic;
    Tick int_delay;
};

#endif // __PARAMS__AmbaIntDevice__
