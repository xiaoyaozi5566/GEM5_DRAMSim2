#ifndef __PARAMS__System__
#define __PARAMS__System__

class System;

#include <cstddef>
#include <string>
#include <cstddef>
#include "enums/MemoryMode.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <vector>
#include "params/AbstractMemory.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <string>
#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"

#include "params/MemObject.hh"

#include "enums/MemoryMode.hh"

struct SystemParams
    : public MemObjectParams
{
    System * create();
    std::string kernel;
    Enums::MemoryMode mem_mode;
    Counter work_end_ckpt_count;
    uint64_t init_param;
    std::vector< AbstractMemory * > memories;
    uint64_t load_addr_mask;
    std::string symbolfile;
    std::string readfile;
    Counter work_cpus_ckpt_count;
    Counter work_begin_exit_count;
    int work_begin_cpu_id_exit;
    Counter work_begin_ckpt_count;
    Counter work_end_exit_count;
    std::string boot_osflags;
    int work_item_id;
    int num_work_ids;
    unsigned int port_system_port_connection_count;
};

#endif // __PARAMS__System__
