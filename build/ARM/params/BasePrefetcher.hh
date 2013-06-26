#ifndef __PARAMS__BasePrefetcher__
#define __PARAMS__BasePrefetcher__

class BasePrefetcher;

#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/System.hh"
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct BasePrefetcherParams
    : public SimObjectParams
{
    bool data_accesses_only;
    Tick latency;
    bool use_master_id;
    int degree;
    System * sys;
    bool serial_squash;
    bool cross_pages;
    int size;
};

#endif // __PARAMS__BasePrefetcher__
