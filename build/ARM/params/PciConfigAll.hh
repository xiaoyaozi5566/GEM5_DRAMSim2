#ifndef __PARAMS__PciConfigAll__
#define __PARAMS__PciConfigAll__

class PciConfigAll;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/Platform.hh"

#include "params/PioDevice.hh"

struct PciConfigAllParams
    : public PioDeviceParams
{
    PciConfigAll * create();
    uint8_t bus;
    uint32_t size;
    Tick pio_latency;
    Platform * platform;
};

#endif // __PARAMS__PciConfigAll__
