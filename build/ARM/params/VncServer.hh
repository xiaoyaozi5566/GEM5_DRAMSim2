#ifndef __PARAMS__VncServer__
#define __PARAMS__VncServer__

class VncServer;

#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct VncServerParams
    : public SimObjectParams
{
    VncServer * create();
    bool frame_capture;
    int number;
    uint16_t port;
};

#endif // __PARAMS__VncServer__
