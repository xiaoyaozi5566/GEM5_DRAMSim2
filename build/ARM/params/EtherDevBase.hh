#ifndef __PARAMS__EtherDevBase__
#define __PARAMS__EtherDevBase__

class EtherDevBase;

#include <cstddef>
#include "base/inet.hh"
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

#include "params/EtherDevice.hh"

struct EtherDevBaseParams
    : public EtherDeviceParams
{
    Net::EthAddr hardware_address;
    Tick rx_delay;
    uint64_t tx_fifo_size;
    Tick dma_write_factor;
    Tick dma_write_delay;
    bool rx_filter;
    Tick dma_read_delay;
    Tick intr_delay;
    Tick dma_read_factor;
    uint64_t rx_fifo_size;
    bool rx_thread;
    bool tx_thread;
    Tick tx_delay;
    bool rss;
};

#endif // __PARAMS__EtherDevBase__
