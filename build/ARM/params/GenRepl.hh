#ifndef __PARAMS__GenRepl__
#define __PARAMS__GenRepl__

class GenRepl;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/Repl.hh"

struct GenReplParams
    : public ReplParams
{
    GenRepl * create();
    int pool_res;
    int fresh_res;
    int num_pools;
};

#endif // __PARAMS__GenRepl__
