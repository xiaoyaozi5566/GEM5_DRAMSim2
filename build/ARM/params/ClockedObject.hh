#ifndef __PARAMS__ClockedObject__
#define __PARAMS__ClockedObject__

class ClockedObject;

#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct ClockedObjectParams
    : public SimObjectParams
{
    Tick clock;
};

#endif // __PARAMS__ClockedObject__
