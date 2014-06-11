#include "mem/cache/tags/lru.hh"

class WPLRU : public LRU{
    private:

    public:
    WPLRU( unsigned _numSets, unsigned _blkSize, unsigned _assoc,
            unsigned _hit_latency, unsigned num_tcs );

    protected:
    CacheSet **sets;
    unsigned num_tcs;
    virtual void init_sets();
    virtual CacheSet get_set( int setnum, uint64_t tid, Addr addr );
    // virtual void cleanupRefs();

    virtual int assoc_of_tc( int tcid );
};
