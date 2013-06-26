%module(package="m5.internal") param_DRAMSim2

%{
#include "params/DRAMSim2.hh"
#include <cstddef>
#include <string>
#include <cstddef>
#include <string>
#include <cstddef>
#include <string>
#include <cstddef>
#include "base/types.hh"
#include <cstddef>
#include <cstddef>
#include <string>
/**
  * This is a workaround for bug in swig. Prior to gcc 4.6.1 the STL
  * headers like vector, string, etc. used to automatically pull in
  * the cstddef header but starting with gcc 4.6.1 they no longer do.
  * This leads to swig generated a file that does not compile so we
  * explicitly include cstddef. Additionally, including version 2.0.4,
  * swig uses ptrdiff_t without the std:: namespace prefix which is
  * required with gcc 4.6.1. We explicitly provide access to it.
  */
#include <cstddef>
using std::ptrdiff_t;
%}

%include "std_string.i"
%include "std_string.i"
%include "std_string.i"
%import "stdint.i"
%import "base/types.hh"
%include "std_string.i"

%import "python/m5/internal/param_DRAMSim2Wrapper.i"


// stop swig from creating/wrapping default ctor/dtor
%nodefault DRAMSim2;
class DRAMSim2
    : public DRAMSim2Wrapper
{
  public:
};

%include "params/DRAMSim2.hh"
