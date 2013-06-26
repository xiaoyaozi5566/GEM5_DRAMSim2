#include "sim/init.hh"

extern "C" {
    void init_param_GHBPrefetcher();
}

EmbeddedSwig embed_swig_param_GHBPrefetcher(init_param_GHBPrefetcher);
