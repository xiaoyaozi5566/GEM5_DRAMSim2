#ifndef __PARAMS__CoherentBus__
#define __PARAMS__CoherentBus__

class CoherentBus;


#include "params/BaseBus.hh"

struct CoherentBusParams
    : public BaseBusParams
{
    CoherentBus * create();
};

#endif // __PARAMS__CoherentBus__
