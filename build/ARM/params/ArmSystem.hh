#ifndef __PARAMS__ArmSystem__
#define __PARAMS__ArmSystem__

class ArmSystem;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"

#include "params/System.hh"

struct ArmSystemParams
    : public SystemParams
{
    ArmSystem * create();
    Addr gic_cpu_addr;
    uint32_t midr_regval;
    bool multi_proc;
    std::string boot_loader;
    Addr flags_addr;
};

#endif // __PARAMS__ArmSystem__
