#ifndef __PARAMS__GHBPrefetcher__
#define __PARAMS__GHBPrefetcher__

class GHBPrefetcher;


#include "params/BasePrefetcher.hh"

struct GHBPrefetcherParams
    : public BasePrefetcherParams
{
    GHBPrefetcher * create();
};

#endif // __PARAMS__GHBPrefetcher__
