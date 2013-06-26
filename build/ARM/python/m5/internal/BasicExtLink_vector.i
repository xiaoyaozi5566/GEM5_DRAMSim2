%module(package="m5.internal") BasicExtLink_vector
%{
#include "params/BasicExtLink.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_BasicExtLink.i"

%include "std_vector.i"

%typemap(in) std::vector< BasicExtLink * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(BasicExtLink *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< BasicExtLink * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(BasicExtLink * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_BasicExtLink) std::vector< BasicExtLink * >;
