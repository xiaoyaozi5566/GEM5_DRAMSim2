#include "MemoryControllerFR.h"
#include "MemoryController.h"
#include "CommandQueueFR.h"
#include "AddressMapping.h"
#include <iomanip>
using namespace DRAMSim;

MemoryControllerFR::MemoryControllerFR(MemorySystem *parent, 
        CSVWriter &csvOut_, ostream &dramsim_log_, 
        const string &outputFilename_,
        bool genTrace_,
        const string &traceFilename_,
        int num_pids_,
        bool fixAddr) :
    MemoryController(parent,csvOut_,dramsim_log_,outputFilename_, genTrace_, traceFilename_, num_pids_, fixAddr)
{

    commandQueue = new CommandQueueFR(bankStates,dramsim_log_,num_pids_);
}