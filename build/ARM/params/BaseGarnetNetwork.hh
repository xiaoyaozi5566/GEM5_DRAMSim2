#ifndef __PARAMS__BaseGarnetNetwork__
#define __PARAMS__BaseGarnetNetwork__

class BaseGarnetNetwork;

#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/FaultModel.hh"

#include "params/RubyNetwork.hh"

struct BaseGarnetNetworkParams
    : public RubyNetworkParams
{
    bool enable_fault_model;
    int ni_flit_size;
    int vcs_per_vnet;
    FaultModel * fault_model;
};

#endif // __PARAMS__BaseGarnetNetwork__
