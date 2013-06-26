#ifndef __PARAMS__RubyNetwork__
#define __PARAMS__RubyNetwork__

class Network;

#include <cstddef>
#include "params/RubySystem.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/Topology.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct RubyNetworkParams
    : public SimObjectParams
{
    RubySystem * ruby_system;
    int number_of_virtual_networks;
    Topology * topology;
    int control_msg_size;
};

#endif // __PARAMS__RubyNetwork__
