#ifndef __PARAMS__NSGigE__
#define __PARAMS__NSGigE__

class NSGigE;

#include <cstddef>
#include <cstddef>
#include <cstddef>

#include "params/EtherDevBase.hh"

struct NSGigEParams
    : public EtherDevBaseParams
{
    NSGigE * create();
    bool dma_no_allocate;
    bool dma_data_free;
    bool dma_desc_free;
};

#endif // __PARAMS__NSGigE__
