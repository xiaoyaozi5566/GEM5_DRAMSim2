%module(package="m5.internal") NetworkLink_d_vector
%{
#include "params/NetworkLink_d.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_NetworkLink_d.i"

%include "std_vector.i"

%typemap(in) std::vector< NetworkLink_d * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(NetworkLink_d *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< NetworkLink_d * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(NetworkLink_d * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_NetworkLink_d) std::vector< NetworkLink_d * >;
