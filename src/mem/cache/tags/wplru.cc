#include "mem/cache/tags/wplru.hh"

WPLRU::WPLRU( unsigned _numSets,
        unsigned _blkSize,
        unsigned _assoc,
        unsigned _hit_latency )
    : LRU(_numSets, _blkSize, _assoc, _hit_latency )
{
    fprintf(stderr, "\x1B[36mWPLRU was born\n\x1B[0m");
}
