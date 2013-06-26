%module(package="m5.internal") param_Topology

%{
#include "params/Topology.hh"
#include <vector>
#include "params/BasicIntLink.hh"
#include <vector>
#include "params/BasicRouter.hh"
#include <cstddef>
#include <cstddef>
#include <string>
#include <vector>
#include "params/BasicExtLink.hh"
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

%import "BasicIntLink_vector.i"
%import "BasicRouter_vector.i"
%include "std_string.i"
%import "BasicExtLink_vector.i"

%import "python/m5/internal/param_SimObject.i"


// stop swig from creating/wrapping default ctor/dtor
%nodefault Topology;
class Topology
    : public SimObject
{
  public:
};

%include "params/Topology.hh"
