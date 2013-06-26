%module(package="m5.internal") AddrRange_vector
%{
#include "base/types.hh"
#include "base/range.hh"
%}

%include "std_container.i"

%import "stdint.i"
%import "base/types.hh"
%import "python/swig/range.i"

%include "std_vector.i"

%typemap(in) std::vector< Range< Addr > >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(Range< Addr >), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< Range< Addr > >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(Range< Addr > *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_AddrRange) std::vector< Range< Addr > >;
