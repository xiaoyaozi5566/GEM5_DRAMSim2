#ifndef __PARAMS__NetworkLink_d__
#define __PARAMS__NetworkLink_d__

class NetworkLink_d;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct NetworkLink_dParams
    : public SimObjectParams
{
    NetworkLink_d * create();
    int virt_nets;
    int link_id;
    int vcs_per_vnet;
    int channel_width;
    int link_latency;
};

#endif // __PARAMS__NetworkLink_d__
