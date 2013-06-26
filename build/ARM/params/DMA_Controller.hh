#ifndef __PARAMS__DMA_Controller__
#define __PARAMS__DMA_Controller__

class DMA_Controller;

#include <cstddef>
#include "params/DMASequencer.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/RubyController.hh"

struct DMA_ControllerParams
    : public RubyControllerParams
{
    DMA_Controller * create();
    DMASequencer * dma_sequencer;
    int request_latency;
};

#endif // __PARAMS__DMA_Controller__
