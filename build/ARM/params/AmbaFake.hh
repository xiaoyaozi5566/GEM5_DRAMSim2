#ifndef __PARAMS__AmbaFake__
#define __PARAMS__AmbaFake__

class AmbaFake;

#include <cstddef>

#include "params/AmbaDevice.hh"

struct AmbaFakeParams
    : public AmbaDeviceParams
{
    AmbaFake * create();
    bool ignore_access;
};

#endif // __PARAMS__AmbaFake__
