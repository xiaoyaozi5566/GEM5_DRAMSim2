#include "sim/init.hh"

extern "C" {
    void init_param_RubyProfiler();
}

EmbeddedSwig embed_swig_param_RubyProfiler(init_param_RubyProfiler);
