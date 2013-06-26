#ifndef __PARAMS__NoncoherentBus__
#define __PARAMS__NoncoherentBus__

class NoncoherentBus;


#include "params/BaseBus.hh"

struct NoncoherentBusParams
    : public BaseBusParams
{
    NoncoherentBus * create();
};

#endif // __PARAMS__NoncoherentBus__
