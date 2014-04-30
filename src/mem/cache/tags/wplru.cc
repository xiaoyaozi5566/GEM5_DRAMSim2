#include "mem/cache/tags/wplru.hh"
#include "base/intmath.hh"
#include "debug/CacheRepl.hh"
#include "mem/cache/tags/cacheset.hh"
#include "mem/cache/tags/lru.hh"
#include "mem/cache/base.hh"
#include "sim/core.hh"
#include <typeinfo>

WPLRU::WPLRU( unsigned _numSets,
        unsigned _blkSize,
        unsigned _assoc,
        unsigned _hit_latency )
    : LRU(_numSets, _blkSize, _assoc, _hit_latency )
{
}

