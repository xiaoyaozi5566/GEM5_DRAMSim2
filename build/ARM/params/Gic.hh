#ifndef __PARAMS__Gic__
#define __PARAMS__Gic__

class Gic;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/Platform.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/PioDevice.hh"

struct GicParams
    : public PioDeviceParams
{
    Gic * create();
    uint32_t it_lines;
    Addr dist_addr;
    Tick cpu_pio_delay;
    Tick dist_pio_delay;
    Addr cpu_addr;
    Platform * platform;
    Tick int_latency;
};

#endif // __PARAMS__Gic__
