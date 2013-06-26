#include "sim/init.hh"

extern "C" {
    void init_param_Gic();
}

EmbeddedSwig embed_swig_param_Gic(init_param_Gic);
