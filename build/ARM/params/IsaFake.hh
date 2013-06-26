#ifndef __PARAMS__IsaFake__
#define __PARAMS__IsaFake__

class IsaFake;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <string>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/BasicPioDevice.hh"

struct IsaFakeParams
    : public BasicPioDeviceParams
{
    IsaFake * create();
    uint8_t ret_data8;
    bool update_data;
    std::string warn_access;
    bool ret_bad_addr;
    uint64_t ret_data64;
    bool fake_mem;
    Addr pio_size;
    uint32_t ret_data32;
    uint16_t ret_data16;
};

#endif // __PARAMS__IsaFake__
