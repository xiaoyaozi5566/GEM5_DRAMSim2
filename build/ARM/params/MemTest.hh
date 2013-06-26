#ifndef __PARAMS__MemTest__
#define __PARAMS__MemTest__

class MemTest;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/System.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>

#include "params/MemObject.hh"

struct MemTestParams
    : public MemObjectParams
{
    MemTest * create();
    int percent_uncacheable;
    int percent_functional;
    Counter max_loads;
    int percent_dest_unaligned;
    int percent_reads;
    System * sys;
    Addr trace_addr;
    int percent_source_unaligned;
    bool issue_dmas;
    int memory_size;
    bool atomic;
    Counter progress_interval;
    bool suppress_func_warnings;
    unsigned int port_test_connection_count;
    unsigned int port_functional_connection_count;
};

#endif // __PARAMS__MemTest__
