#include "sim/init.hh"

extern "C" {
    void init_param_DRAMSim2Wrapper();
}

EmbeddedSwig embed_swig_param_DRAMSim2Wrapper(init_param_DRAMSim2Wrapper);
