%module(package="m5.internal") FUDesc_vector
%{
#include "params/FUDesc.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_FUDesc.i"

%include "std_vector.i"

%typemap(in) std::vector< FUDesc * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(FUDesc *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< FUDesc * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(FUDesc * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_FUDesc) std::vector< FUDesc * >;
