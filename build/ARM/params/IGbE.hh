#ifndef __PARAMS__IGbE__
#define __PARAMS__IGbE__

class IGbE;

#include <cstddef>
#include "base/inet.hh"
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
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/EtherDevice.hh"

struct IGbEParams
    : public EtherDeviceParams
{
    IGbE * create();
    Net::EthAddr hardware_address;
    Tick fetch_delay;
    bool use_flow_control;
    uint16_t phy_pid;
    uint64_t tx_fifo_size;
    int tx_desc_cache_size;
    Tick rx_write_delay;
    int rx_desc_cache_size;
    uint64_t rx_fifo_size;
    Tick wb_delay;
    Tick tx_read_delay;
    uint16_t phy_epid;
    Tick fetch_comp_delay;
    Tick wb_comp_delay;
};

#endif // __PARAMS__IGbE__
