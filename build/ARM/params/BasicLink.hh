#ifndef __PARAMS__BasicLink__
#define __PARAMS__BasicLink__

class BasicLink;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/SimObject.hh"

struct BasicLinkParams
    : public SimObjectParams
{
    BasicLink * create();
    int latency;
    int bandwidth_factor;
    int weight;
    int link_id;
};

#endif // __PARAMS__BasicLink__
