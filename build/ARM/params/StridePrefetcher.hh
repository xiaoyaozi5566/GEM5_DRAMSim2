#ifndef __PARAMS__StridePrefetcher__
#define __PARAMS__StridePrefetcher__

class StridePrefetcher;


#include "params/BasePrefetcher.hh"

struct StridePrefetcherParams
    : public BasePrefetcherParams
{
    StridePrefetcher * create();
};

#endif // __PARAMS__StridePrefetcher__
