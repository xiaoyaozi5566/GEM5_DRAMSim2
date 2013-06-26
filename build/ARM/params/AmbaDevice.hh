#ifndef __PARAMS__AmbaDevice__
#define __PARAMS__AmbaDevice__

class AmbaDevice;

#include <cstddef>
#include "base/types.hh"

#include "params/BasicPioDevice.hh"

struct AmbaDeviceParams
    : public BasicPioDeviceParams
{
    uint32_t amba_id;
};

#endif // __PARAMS__AmbaDevice__
