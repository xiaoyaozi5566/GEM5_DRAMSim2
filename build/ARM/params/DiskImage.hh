#ifndef __PARAMS__DiskImage__
#define __PARAMS__DiskImage__

class DiskImage;

#include <cstddef>
#include <cstddef>
#include <string>

#include "params/SimObject.hh"

struct DiskImageParams
    : public SimObjectParams
{
    bool read_only;
    std::string image_file;
};

#endif // __PARAMS__DiskImage__
