#ifndef __PARAMS__Pl111__
#define __PARAMS__Pl111__

class Pl111;

#include <cstddef>
#include "params/VncServer.hh"

#include "params/AmbaDmaDevice.hh"

struct Pl111Params
    : public AmbaDmaDeviceParams
{
    Pl111 * create();
    VncServer * vnc;
};

#endif // __PARAMS__Pl111__
