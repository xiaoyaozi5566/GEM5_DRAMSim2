%module(package="m5.internal") BasicRouter_vector
%{
#include "params/BasicRouter.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_BasicRouter.i"

%include "std_vector.i"

%typemap(in) std::vector< BasicRouter * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(BasicRouter *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< BasicRouter * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(BasicRouter * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_BasicRouter) std::vector< BasicRouter * >;
