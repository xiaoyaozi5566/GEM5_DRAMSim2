#include "sim/init.hh"

extern "C" {
    void init_param_CommMonitor();
}

EmbeddedSwig embed_swig_param_CommMonitor(init_param_CommMonitor);
