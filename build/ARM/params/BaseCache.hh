#ifndef __PARAMS__BaseCache__
#define __PARAMS__BaseCache__

class BaseCache;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/Repl.hh"
#include <cstddef>
#include <vector>
#include "base/types.hh"
#include "base/range.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/System.hh"
#include <cstddef>
#include "params/BasePrefetcher.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"

#include "params/MemObject.hh"

struct BaseCacheParams
    : public MemObjectParams
{
    BaseCache * create();
    Tick latency;
    Repl * repl;
    bool two_queue;
    std::vector< Range< Addr > > addr_ranges;
    Cycles hash_delay;
    int tgts_per_mshr;
    System * system;
    BasePrefetcher * prefetcher;
    bool prioritizeRequests;
    Addr trace_addr;
    Counter max_miss_count;
    int write_buffers;
    int subblock_size;
    bool prefetch_on_access;
    bool is_top_level;
    int assoc;
    int block_size;
    int mshrs;
    bool forward_snoops;
    uint64_t size;
    unsigned int port_mem_side_connection_count;
    unsigned int port_cpu_side_connection_count;
};

#endif // __PARAMS__BaseCache__
