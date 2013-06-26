%module(package="m5.internal") IdeDisk_vector
%{
#include "params/IdeDisk.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_IdeDisk.i"

%include "std_vector.i"

%typemap(in) std::vector< IdeDisk * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(IdeDisk *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< IdeDisk * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(IdeDisk * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_IdeDisk) std::vector< IdeDisk * >;
