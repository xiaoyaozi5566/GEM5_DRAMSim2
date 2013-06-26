#ifndef __PARAMS__L1Cache_Controller__
#define __PARAMS__L1Cache_Controller__

class L1Cache_Controller;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "params/RubyCache.hh"
#include <cstddef>
#include "params/RubySequencer.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/RubyController.hh"

struct L1Cache_ControllerParams
    : public RubyControllerParams
{
    L1Cache_Controller * create();
    int cache_response_latency;
    bool send_evictions;
    CacheMemory * cacheMemory;
    Sequencer * sequencer;
    int issue_latency;
};

#endif // __PARAMS__L1Cache_Controller__
