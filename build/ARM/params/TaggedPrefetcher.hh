#ifndef __PARAMS__TaggedPrefetcher__
#define __PARAMS__TaggedPrefetcher__

class TaggedPrefetcher;


#include "params/BasePrefetcher.hh"

struct TaggedPrefetcherParams
    : public BasePrefetcherParams
{
    TaggedPrefetcher * create();
};

#endif // __PARAMS__TaggedPrefetcher__
