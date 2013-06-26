%module(package="m5.internal") Int_vector
%{
#include "base/types.hh"
%}

%include "std_container.i"

%import "stdint.i"
%import "base/types.hh"

%include "std_vector.i"

%typemap(in) std::vector< int >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(int), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< int >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(int *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_Int) std::vector< int >;
