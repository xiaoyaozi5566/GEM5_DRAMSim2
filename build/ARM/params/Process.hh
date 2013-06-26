#ifndef __PARAMS__Process__
#define __PARAMS__Process__

class Process;

#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <string>
#include <cstddef>
#include <string>
#include <cstddef>
#include "params/System.hh"

#include "params/SimObject.hh"

struct ProcessParams
    : public SimObjectParams
{
    std::string input;
    uint64_t max_stack_size;
    std::string output;
    std::string errout;
    System * system;
};

#endif // __PARAMS__Process__
