#include "mem/cache/base.hh"
#include "mem/cache/cache.hh"
#include "params/BaseCache.hh"
#include "stdio.h"

typedef BaseCacheParams Params;
template <class TagStore>
class SplitRPortCache : public SplitMSHRCache<TagStore>
{

    private:
    // Number of timing comparments / security domains.
    // Derived from BaseCache.py
    int num_tcs;

    public:
    SplitRPortCache( const Params *p, TagStore *tags );

    protected:
    // Any functions from Cache that are overridden are added here. If they are 
    // called within the cache.hh you will want to add the virtual keyword. 
    // This makes it so that if you instantiate a SplitRPortCache, any 
    // calls to the virtual function will correctly use the version written in 
    // this module, even if the call takes place in the parent class. As an 
    // example, an arbitrary function of cache is shown here.
    //
    // It's very important that the virtual keyword is added to the parent 
    // declaration in cache.hh too.
    //
    //TODO delete this if not needed
    virtual PacketPtr getTimingPacket( int threadID );

};
