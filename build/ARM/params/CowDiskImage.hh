#ifndef __PARAMS__CowDiskImage__
#define __PARAMS__CowDiskImage__

class CowDiskImage;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/DiskImage.hh"

#include "params/DiskImage.hh"

struct CowDiskImageParams
    : public DiskImageParams
{
    CowDiskImage * create();
    int table_size;
    DiskImage * child;
};

#endif // __PARAMS__CowDiskImage__
