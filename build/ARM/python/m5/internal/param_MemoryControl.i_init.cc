#include "sim/init.hh"

extern "C" {
    void init_param_MemoryControl();
}

EmbeddedSwig embed_swig_param_MemoryControl(init_param_MemoryControl);
