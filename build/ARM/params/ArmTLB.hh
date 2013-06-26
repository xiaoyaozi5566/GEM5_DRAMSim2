#ifndef __PARAMS__ArmTLB__
#define __PARAMS__ArmTLB__

namespace ArmISA {
class TLB;
} // namespace ArmISA

#include <cstddef>
#include "params/ArmTableWalker.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct ArmTLBParams
    : public SimObjectParams
{
    ArmISA::TLB * create();
    ArmISA::TableWalker * walker;
    int size;
};

#endif // __PARAMS__ArmTLB__
