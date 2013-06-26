#ifndef __PARAMS__RealView__
#define __PARAMS__RealView__

class RealView;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/System.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/Platform.hh"

struct RealViewParams
    : public PlatformParams
{
    RealView * create();
    Addr max_mem_size;
    Addr pci_cfg_base;
    System * system;
    Addr mem_start_addr;
};

#endif // __PARAMS__RealView__
