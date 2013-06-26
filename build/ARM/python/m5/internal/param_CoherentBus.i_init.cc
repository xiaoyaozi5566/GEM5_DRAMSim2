#include "sim/init.hh"

extern "C" {
    void init_param_CoherentBus();
}

EmbeddedSwig embed_swig_param_CoherentBus(init_param_CoherentBus);
