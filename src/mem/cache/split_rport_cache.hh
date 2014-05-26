#include "mem/cache/base.hh"
#include "mem/cache/cache.hh"
#include "params/BaseCache.hh"
#include "stdio.h"

typedef BaseCacheParams Params;
template <class TagStore>
class SplitRPortCache : public SplitMSHRCache<TagStore>
{

    public:
    SplitRPortCache( const Params *p, TagStore *tags );

    protected:
    
	virtual bool isSplitRPort() {return true;}

};
