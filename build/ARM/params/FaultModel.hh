#ifndef __PARAMS__FaultModel__
#define __PARAMS__FaultModel__

class FaultModel;

#include <vector>
#include "base/types.hh"
#include <vector>

#include "params/SimObject.hh"

struct FaultModelParams
    : public SimObjectParams
{
    FaultModel * create();
    std::vector< int > temperature_weights_database;
    std::vector< double > baseline_fault_vector_database;
};

#endif // __PARAMS__FaultModel__
