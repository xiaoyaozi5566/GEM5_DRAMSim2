#include "sim/init.hh"

extern "C" {
    void init_param_Repl();
}

EmbeddedSwig embed_swig_param_Repl(init_param_Repl);
