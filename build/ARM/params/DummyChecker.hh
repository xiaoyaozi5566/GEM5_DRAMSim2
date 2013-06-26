#ifndef __PARAMS__DummyChecker__
#define __PARAMS__DummyChecker__

class DummyChecker;


#include "params/BaseCPU.hh"

struct DummyCheckerParams
    : public BaseCPUParams
{
    DummyChecker * create();
};

#endif // __PARAMS__DummyChecker__
