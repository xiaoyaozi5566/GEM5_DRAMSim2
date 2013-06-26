#ifndef __PARAMS__Root__
#define __PARAMS__Root__

class Root;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct RootParams
    : public SimObjectParams
{
    Root * create();
    Tick time_sync_period;
    bool time_sync_enable;
    bool full_system;
    Tick time_sync_spin_threshold;
};

#endif // __PARAMS__Root__
