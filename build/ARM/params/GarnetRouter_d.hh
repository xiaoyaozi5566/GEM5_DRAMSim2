#ifndef __PARAMS__GarnetRouter_d__
#define __PARAMS__GarnetRouter_d__

class Router_d;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/BasicRouter.hh"

struct GarnetRouter_dParams
    : public BasicRouterParams
{
    Router_d * create();
    int virt_nets;
    int vcs_per_vnet;
};

#endif // __PARAMS__GarnetRouter_d__
