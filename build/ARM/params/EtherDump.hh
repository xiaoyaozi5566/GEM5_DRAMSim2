#ifndef __PARAMS__EtherDump__
#define __PARAMS__EtherDump__

class EtherDump;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <string>

#include "params/SimObject.hh"

struct EtherDumpParams
    : public SimObjectParams
{
    EtherDump * create();
    int maxlen;
    std::string file;
};

#endif // __PARAMS__EtherDump__
