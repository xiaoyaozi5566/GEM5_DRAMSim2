#ifndef __PARAMS__BasicRouter__
#define __PARAMS__BasicRouter__

class BasicRouter;

#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct BasicRouterParams
    : public SimObjectParams
{
    BasicRouter * create();
    int router_id;
};

#endif // __PARAMS__BasicRouter__
