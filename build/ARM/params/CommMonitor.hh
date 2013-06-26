#ifndef __PARAMS__CommMonitor__
#define __PARAMS__CommMonitor__

class CommMonitor;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
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
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/MemObject.hh"

struct CommMonitorParams
    : public MemObjectParams
{
    CommMonitor * create();
    Addr write_addr_mask;
    unsigned bandwidth_bins;
    bool disable_itt_dists;
    bool disable_outstanding_hists;
    bool disable_transaction_hists;
    Tick sample_period;
    bool disable_latency_hists;
    bool disable_burst_length_hists;
    bool disable_bandwidth_hists;
    Addr read_addr_mask;
    unsigned latency_bins;
    unsigned outstanding_bins;
    bool disable_addr_dists;
    unsigned burst_length_bins;
    unsigned transaction_bins;
    unsigned itt_bins;
    Tick itt_max_bin;
    unsigned int port_slave_connection_count;
    unsigned int port_master_connection_count;
};

#endif // __PARAMS__CommMonitor__
