#include "sim/init.hh"

extern "C" {
    void init_param_DRAMSim2();
}

EmbeddedSwig embed_swig_param_DRAMSim2(init_param_DRAMSim2);
