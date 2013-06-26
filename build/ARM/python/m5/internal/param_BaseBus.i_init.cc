#include "sim/init.hh"

extern "C" {
    void init_param_BaseBus();
}

EmbeddedSwig embed_swig_param_BaseBus(init_param_BaseBus);
