#ifndef __PARAMS__LinuxArmSystem__
#define __PARAMS__LinuxArmSystem__

class LinuxArmSystem;

#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "enums/ArmMachineType.hh"

#include "params/ArmSystem.hh"

#include "enums/ArmMachineType.hh"

struct LinuxArmSystemParams
    : public ArmSystemParams
{
    LinuxArmSystem * create();
    bool early_kernel_symbols;
    Addr atags_addr;
    Enums::ArmMachineType machine_type;
};

#endif // __PARAMS__LinuxArmSystem__
