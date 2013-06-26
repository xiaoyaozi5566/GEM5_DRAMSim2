#ifndef __PARAMS__MemoryControl__
#define __PARAMS__MemoryControl__

class MemoryControl;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/RubySystem.hh"

#include "params/ClockedObject.hh"

struct MemoryControlParams
    : public ClockedObjectParams
{
    int version;
    RubySystem * ruby_system;
};

#endif // __PARAMS__MemoryControl__
