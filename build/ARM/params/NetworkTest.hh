#ifndef __PARAMS__NetworkTest__
#define __PARAMS__NetworkTest__

class NetworkTest;

#include <cstddef>
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
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>

#include "params/MemObject.hh"

struct NetworkTestParams
    : public MemObjectParams
{
    NetworkTest * create();
    bool fixed_pkts;
    int block_offset;
    int num_memories;
    int precision;
    System * system;
    int memory_size;
    Counter max_packets;
    Counter traffic_type;
    int sim_cycles;
    double inj_rate;
    unsigned int port_test_connection_count;
};

#endif // __PARAMS__NetworkTest__
