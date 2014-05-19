#include "mem/cache/base.hh"
#include "mem/cache/cache.hh"
#include "params/BaseCache.hh"
#include "stdio.h"

typedef BaseCacheParams Params;
template <class TagStore>
class SplitMSHRCache : public Cache<TagStore>
{
    private:
        MSHRQueue** mshrQueues;
        MSHRQueue** writeBuffers;

    public:
    SplitMSHRCache( const Params *p, TagStore *tags );

    protected:
    virtual MSHRQueue* getMSHRQueue( int threadID ){
        return mshrQueues[threadID];
    }
    virtual MSHRQueue* getWriteBuffer( int threadID ){
        return writeBuffers[threadID];
    }
};
