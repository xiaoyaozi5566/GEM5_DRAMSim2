%module(package="m5.internal") BasicIntLink_vector
%{
#include "params/BasicIntLink.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_BasicIntLink.i"

%include "std_vector.i"

%typemap(in) std::vector< BasicIntLink * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(BasicIntLink *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< BasicIntLink * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(BasicIntLink * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_BasicIntLink) std::vector< BasicIntLink * >;
