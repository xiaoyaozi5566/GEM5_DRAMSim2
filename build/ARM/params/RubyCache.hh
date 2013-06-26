#ifndef __PARAMS__RubyCache__
#define __PARAMS__RubyCache__

class CacheMemory;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct RubyCacheParams
    : public SimObjectParams
{
    CacheMemory * create();
    int latency;
    std::string replacement_policy;
    int tagArrayBanks;
    int dataArrayBanks;
    Cycles tagAccessLatency;
    bool is_icache;
    int assoc;
    int start_index_bit;
    Cycles dataAccessLatency;
    bool resourceStalls;
    uint64_t size;
};

#endif // __PARAMS__RubyCache__
