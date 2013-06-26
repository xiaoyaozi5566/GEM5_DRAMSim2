#ifndef __PARAMS__RubyProfiler__
#define __PARAMS__RubyProfiler__

class Profiler;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/RubySystem.hh"
#include <cstddef>
#include <cstddef>

#include "params/SimObject.hh"

struct RubyProfilerParams
    : public SimObjectParams
{
    Profiler * create();
    int num_of_sequencers;
    RubySystem * ruby_system;
    bool hot_lines;
    bool all_instructions;
};

#endif // __PARAMS__RubyProfiler__
