#ifndef __PARAMS__Topology__
#define __PARAMS__Topology__

class Topology;

#include <vector>
#include "params/BasicIntLink.hh"
#include <vector>
#include "params/BasicRouter.hh"
#include <cstddef>
#include <cstddef>
#include <string>
#include <vector>
#include "params/BasicExtLink.hh"

#include "params/SimObject.hh"

struct TopologyParams
    : public SimObjectParams
{
    Topology * create();
    std::vector< BasicIntLink * > int_links;
    std::vector< BasicRouter * > routers;
    bool print_config;
    std::string description;
    std::vector< BasicExtLink * > ext_links;
};

#endif // __PARAMS__Topology__
