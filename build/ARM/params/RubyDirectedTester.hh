#ifndef __PARAMS__RubyDirectedTester__
#define __PARAMS__RubyDirectedTester__

class RubyDirectedTester;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/DirectedGenerator.hh"

#include "params/MemObject.hh"

struct RubyDirectedTesterParams
    : public MemObjectParams
{
    RubyDirectedTester * create();
    int requests_to_complete;
    DirectedGenerator * generator;
    unsigned int port_cpuPort_connection_count;
};

#endif // __PARAMS__RubyDirectedTester__
