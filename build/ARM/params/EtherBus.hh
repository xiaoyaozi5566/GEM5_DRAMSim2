#ifndef __PARAMS__EtherBus__
#define __PARAMS__EtherBus__

class EtherBus;

#include <cstddef>
#include <cstddef>
#include "params/EtherDump.hh"
#include <cstddef>

#include "params/EtherObject.hh"

struct EtherBusParams
    : public EtherObjectParams
{
    EtherBus * create();
    float speed;
    EtherDump * dump;
    bool loopback;
};

#endif // __PARAMS__EtherBus__
