#ifndef __PARAMS__ArmTableWalker__
#define __PARAMS__ArmTableWalker__

namespace ArmISA {
class TableWalker;
} // namespace ArmISA

#include <cstddef>
#include "params/System.hh"

#include "params/MemObject.hh"

struct ArmTableWalkerParams
    : public MemObjectParams
{
    ArmISA::TableWalker * create();
    System * sys;
    unsigned int port_port_connection_count;
};

#endif // __PARAMS__ArmTableWalker__
