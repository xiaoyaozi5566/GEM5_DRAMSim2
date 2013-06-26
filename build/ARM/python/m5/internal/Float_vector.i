%module(package="m5.internal") Float_vector
%{
%}

%include "std_container.i"


%include "std_vector.i"

%typemap(in) std::vector< double >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(double), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< double >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(double *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_Float) std::vector< double >;
