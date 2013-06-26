#ifndef __PARAMS__AbstractMemory__
#define __PARAMS__AbstractMemory__

class AbstractMemory;

#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include "base/range.hh"
#include <cstddef>
#include <cstddef>
#include <string>
#include <cstddef>

#include "params/MemObject.hh"

struct AbstractMemoryParams
    : public MemObjectParams
{
    bool in_addr_map;
    bool zero;
    Range< Addr > range;
    bool conf_table_reported;
    std::string file;
    bool null;
};

#endif // __PARAMS__AbstractMemory__
