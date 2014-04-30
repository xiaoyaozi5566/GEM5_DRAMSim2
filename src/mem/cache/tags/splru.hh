#include "mem/cache/tags/lru.hh"

class SPLRU : public LRU{
    private:
        CacheSet **sets;
        const int num_sds = 2;

    public:
    SPLRU( unsigned _numSets, unsigned _blkSize, unsigned _assoc,
            unsigned _hit_latency );
    protected:
    virtual void init_sets();
    virtual CacheSet get_set( int setnum, uint64_t tid );
    virtual void cleanupRefs();
};
