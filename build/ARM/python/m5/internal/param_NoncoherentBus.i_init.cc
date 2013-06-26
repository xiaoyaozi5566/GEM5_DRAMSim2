#include "sim/init.hh"

extern "C" {
    void init_param_NoncoherentBus();
}

EmbeddedSwig embed_swig_param_NoncoherentBus(init_param_NoncoherentBus);
