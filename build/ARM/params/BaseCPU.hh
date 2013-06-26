#ifndef __PARAMS__BaseCPU__
#define __PARAMS__BaseCPU__

class BaseCPU;

#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/ArmInterrupts.hh"
#include <cstddef>
#include "params/ArmTLB.hh"
#include <cstddef>
#include "params/System.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/BaseCPU.hh"
#include <vector>
#include "params/Process.hh"
#include <cstddef>
#include <cstddef>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/ArmTLB.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include "params/InstTracer.hh"

#include "params/MemObject.hh"

struct BaseCPUParams
    : public MemObjectParams
{
    Tick profile;
    bool do_quiesce;
    bool defer_registration;
    bool do_statistics_insts;
    unsigned numThreads;
    ArmISA::Interrupts * interrupts;
    ArmISA::TLB * itb;
    System * system;
    Counter max_insts_any_thread;
    Tick function_trace_start;
    int cpu_id;
    BaseCPU * checker;
    std::vector< Process * > workload;
    bool function_trace;
    bool do_checkpoint_insts;
    Tick progress_interval;
    Counter max_insts_all_threads;
    ArmISA::TLB * dtb;
    Counter max_loads_any_thread;
    Counter max_loads_all_threads;
    Trace::InstTracer * tracer;
    unsigned int port_icache_port_connection_count;
    unsigned int port_dcache_port_connection_count;
};

#endif // __PARAMS__BaseCPU__
