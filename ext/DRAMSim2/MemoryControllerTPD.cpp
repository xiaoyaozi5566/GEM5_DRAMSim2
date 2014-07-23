#include "MemoryControllerTPD.h"
using namespace DRAMSim;

MemoryControllerTPD::MemoryControllerTPD(MemorySystem *parent, 
        CSVWriter &csvOut, ostream &dramsim_log, 
        const string &outputFilename,
        unsigned tpTurnLength,
        bool genTrace,
        const string &traceFilename,
        int num_pids,
        bool fixAddr,
        bool diffPeriod,
        int p0Period,
        int p1Period,
        int offset) :
  MemoryControllerTP(
      parent,
      csvOut,
      dramsim_log,
      outputFilename,
      tpTurnLength,
      genTrace,
      traceFilename,
      num_pids,
      fixAddr,
      diffPeriod,
      p0Period,
      p1Period,
      offset)
{
    commandQueue = new CommandQueueTPD(bankStates,dramsim_log,tpTurnLength,
        num_pids, fixAddr, diffPeriod, p0Period, p1Period, offset); 
}
