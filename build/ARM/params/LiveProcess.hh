#ifndef __PARAMS__LiveProcess__
#define __PARAMS__LiveProcess__

class LiveProcess;

#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <vector>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <vector>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <string>

#include "params/Process.hh"

struct LiveProcessParams
    : public ProcessParams
{
    LiveProcess * create();
    std::string executable;
    int uid;
    std::vector< std::string > cmd;
    int pid;
    uint64_t simpoint;
    int egid;
    int euid;
    std::vector< std::string > env;
    int gid;
    int ppid;
    std::string cwd;
};

#endif // __PARAMS__LiveProcess__
