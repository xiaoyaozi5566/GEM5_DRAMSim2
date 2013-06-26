%module(package="m5.internal") Process_vector
%{
#include "params/Process.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_Process.i"

%include "std_vector.i"

%typemap(in) std::vector< Process * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(Process *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< Process * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(Process * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_Process) std::vector< Process * >;
