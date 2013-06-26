#include "sim/init.hh"

extern "C" {
    void init_param_AmbaDevice();
}

EmbeddedSwig embed_swig_param_AmbaDevice(init_param_AmbaDevice);
