#ifndef __PARAMS__RubySequencer__
#define __PARAMS__RubySequencer__

class Sequencer;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/RubyCache.hh"
#include <cstddef>
#include "params/RubyCache.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/RubyPort.hh"

struct RubySequencerParams
    : public RubyPortParams
{
    Sequencer * create();
    int max_outstanding_requests;
    CacheMemory * icache;
    CacheMemory * dcache;
    Cycles deadlock_threshold;
};

#endif // __PARAMS__RubySequencer__
