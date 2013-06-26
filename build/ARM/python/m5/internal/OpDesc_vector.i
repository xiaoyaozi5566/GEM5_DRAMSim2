%module(package="m5.internal") OpDesc_vector
%{
#include "params/OpDesc.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_OpDesc.i"

%include "std_vector.i"

%typemap(in) std::vector< OpDesc * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(OpDesc *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< OpDesc * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(OpDesc * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_OpDesc) std::vector< OpDesc * >;
