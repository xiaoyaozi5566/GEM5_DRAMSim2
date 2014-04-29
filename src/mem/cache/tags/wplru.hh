#include "mem/cache/tags/lru.hh"

class WPLRU : public LRU{
    public:
    WPLRU( unsigned _numSets, unsigned _blkSize, unsigned _assoc,
            unsigned _hit_latency );
};
