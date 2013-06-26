#include "sim/init.hh"

extern "C" {
    void init_param_Topology();
}

EmbeddedSwig embed_swig_param_Topology(init_param_Topology);
