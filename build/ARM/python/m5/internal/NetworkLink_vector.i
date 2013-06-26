%module(package="m5.internal") NetworkLink_vector
%{
#include "params/NetworkLink.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_NetworkLink.i"

%include "std_vector.i"

%typemap(in) std::vector< NetworkLink * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(NetworkLink *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< NetworkLink * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(NetworkLink * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_NetworkLink) std::vector< NetworkLink * >;
