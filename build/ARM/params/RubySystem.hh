#ifndef __PARAMS__RubySystem__
#define __PARAMS__RubySystem__

class RubySystem;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <string>
#include <cstddef>
#include <cstddef>

#include "params/SimObject.hh"

struct RubySystemParams
    : public SimObjectParams
{
    RubySystem * create();
    int random_seed;
    Tick clock;
    uint64_t mem_size;
    int block_size_bytes;
    std::string stats_filename;
    bool randomization;
    bool no_mem_vec;
};

#endif // __PARAMS__RubySystem__
