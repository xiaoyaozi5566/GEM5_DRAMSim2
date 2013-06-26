#include "sim/init.hh"

extern "C" {
    void init_param_GenRepl();
}

EmbeddedSwig embed_swig_param_GenRepl(init_param_GenRepl);
