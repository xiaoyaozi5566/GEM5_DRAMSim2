#ifndef __PARAMS__Terminal__
#define __PARAMS__Terminal__

class Terminal;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/IntrControl.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>

#include "params/SimObject.hh"

struct TerminalParams
    : public SimObjectParams
{
    Terminal * create();
    uint16_t port;
    IntrControl * intr_control;
    int number;
    bool output;
};

#endif // __PARAMS__Terminal__
