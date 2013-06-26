#ifndef __PARAMS__SimpleNetwork__
#define __PARAMS__SimpleNetwork__

class SimpleNetwork;

#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/RubyNetwork.hh"

struct SimpleNetworkParams
    : public RubyNetworkParams
{
    SimpleNetwork * create();
    bool adaptive_routing;
    int endpoint_bandwidth;
    int buffer_size;
};

#endif // __PARAMS__SimpleNetwork__
