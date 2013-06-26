%module(package="m5.internal") CreditLink_d_vector
%{
#include "params/CreditLink_d.hh"
%}

%include "std_container.i"

%import "python/m5/internal/param_CreditLink_d.i"

%include "std_vector.i"

%typemap(in) std::vector< CreditLink_d * >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(CreditLink_d *), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< CreditLink_d * >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(CreditLink_d * *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_CreditLink_d) std::vector< CreditLink_d * >;
