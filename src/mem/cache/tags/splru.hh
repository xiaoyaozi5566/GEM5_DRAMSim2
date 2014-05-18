#include "mem/cache/tags/lru.hh"
#include "params/BaseCache.hh"

class SPLRU : public LRU{
    private:
        CacheSet **sets;

    public:
    SPLRU( unsigned _numSets, unsigned _blkSize, unsigned _assoc,
            unsigned _hit_latency, unsigned num_tcs);
    protected:
    virtual void init_sets();
    virtual CacheSet get_set( int setnum, uint64_t tid, Addr addr );
    virtual void cleanupRefs();
};
