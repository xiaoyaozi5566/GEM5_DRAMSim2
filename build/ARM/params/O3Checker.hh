#ifndef __PARAMS__O3Checker__
#define __PARAMS__O3Checker__

class O3Checker;

#include <cstddef>
#include <cstddef>
#include <cstddef>

#include "params/BaseCPU.hh"

struct O3CheckerParams
    : public BaseCPUParams
{
    O3Checker * create();
    bool updateOnError;
    bool exitOnError;
    bool warnOnlyOnLoadError;
};

#endif // __PARAMS__O3Checker__
