%module(package="m5.internal") AbstractMemory_vector
%{
#include "params/AbstractMemory.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_AbstractMemory.i"

%include "std_vector.i"

%typemap(in) std::vector< AbstractMemory * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(AbstractMemory *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< AbstractMemory * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(AbstractMemory * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_AbstractMemory) std::vector< AbstractMemory * >;
