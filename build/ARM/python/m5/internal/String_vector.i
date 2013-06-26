%module(package="m5.internal") String_vector
%{
#include <string>
%}

%include "std_container.i"

%include "std_string.i"

%include "std_vector.i"

%typemap(in) std::vector< std::string >::value_type {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(std::string), 0) == -1) {
            return NULL;
        }
    }
}

%typemap(in) std::vector< std::string >::value_type * {
    if (SWIG_ConvertPtr($input, (void **)&$1, $1_descriptor, 0) == -1) {
        if (SWIG_ConvertPtr($input, (void **)&$1,
                            $descriptor(std::string *), 0) == -1) {
            return NULL;
        }
    }
}
%template(vector_String) std::vector< std::string >;
