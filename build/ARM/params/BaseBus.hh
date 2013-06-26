#ifndef __PARAMS__BaseBus__
#define __PARAMS__BaseBus__

class BaseBus;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"

#include "params/MemObject.hh"

struct BaseBusParams
    : public MemObjectParams
{
    int width;
    int block_size;
    bool use_default_range;
    Cycles header_cycles;
    unsigned int port_default_connection_count;
    unsigned int port_master_connection_count;
    unsigned int port_slave_connection_count;
};

#endif // __PARAMS__BaseBus__
