#ifndef __PARAMS__RubyPort__
#define __PARAMS__RubyPort__

class RubyPort;

#include <cstddef>
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "params/System.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "params/RubySystem.hh"

#include "params/MemObject.hh"

struct RubyPortParams
    : public MemObjectParams
{
    bool access_phys_mem;
    bool support_inst_reqs;
    bool support_data_reqs;
    System * system;
    bool using_ruby_tester;
    int version;
    bool using_network_tester;
    RubySystem * ruby_system;
    unsigned int port_master_connection_count;
    unsigned int port_slave_connection_count;
    unsigned int port_pio_port_connection_count;
};

#endif // __PARAMS__RubyPort__
