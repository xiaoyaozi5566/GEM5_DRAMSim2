#ifndef __PARAMS__Pl050__
#define __PARAMS__Pl050__

class Pl050;

#include <cstddef>
#include <cstddef>
#include "params/VncServer.hh"

#include "params/AmbaIntDevice.hh"

struct Pl050Params
    : public AmbaIntDeviceParams
{
    Pl050 * create();
    bool is_mouse;
    VncServer * vnc;
};

#endif // __PARAMS__Pl050__
