#ifndef __PARAMS__RubyController__
#define __PARAMS__RubyController__

class AbstractController;

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
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/RubySystem.hh"

#include "params/SimObject.hh"

struct RubyControllerParams
    : public SimObjectParams
{
    int cntrl_id;
    int transitions_per_cycle;
    int recycle_latency;
    int version;
    int buffer_size;
    int number_of_TBEs;
    RubySystem * ruby_system;
};

#endif // __PARAMS__RubyController__
