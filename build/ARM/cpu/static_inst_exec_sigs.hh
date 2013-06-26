
#ifndef __CPU_STATIC_INST_EXEC_SIGS_HH__
#define __CPU_STATIC_INST_EXEC_SIGS_HH__


virtual Fault execute(AtomicSimpleCPU *xc, Trace::InstRecord *traceData) const = 0;
virtual Fault eaComp(AtomicSimpleCPU *xc, Trace::InstRecord *traceData) const
{ panic("eaComp not defined!"); M5_DUMMY_RETURN };
virtual Fault initiateAcc(AtomicSimpleCPU *xc, Trace::InstRecord *traceData) const
{ panic("initiateAcc not defined!"); M5_DUMMY_RETURN };
virtual Fault completeAcc(Packet *pkt, AtomicSimpleCPU *xc,
                          Trace::InstRecord *traceData) const
{ panic("completeAcc not defined!"); M5_DUMMY_RETURN };


virtual Fault execute(O3DynInst *xc, Trace::InstRecord *traceData) const = 0;
virtual Fault eaComp(O3DynInst *xc, Trace::InstRecord *traceData) const
{ panic("eaComp not defined!"); M5_DUMMY_RETURN };
virtual Fault initiateAcc(O3DynInst *xc, Trace::InstRecord *traceData) const
{ panic("initiateAcc not defined!"); M5_DUMMY_RETURN };
virtual Fault completeAcc(Packet *pkt, O3DynInst *xc,
                          Trace::InstRecord *traceData) const
{ panic("completeAcc not defined!"); M5_DUMMY_RETURN };


virtual Fault execute(TimingSimpleCPU *xc, Trace::InstRecord *traceData) const = 0;
virtual Fault eaComp(TimingSimpleCPU *xc, Trace::InstRecord *traceData) const
{ panic("eaComp not defined!"); M5_DUMMY_RETURN };
virtual Fault initiateAcc(TimingSimpleCPU *xc, Trace::InstRecord *traceData) const
{ panic("initiateAcc not defined!"); M5_DUMMY_RETURN };
virtual Fault completeAcc(Packet *pkt, TimingSimpleCPU *xc,
                          Trace::InstRecord *traceData) const
{ panic("completeAcc not defined!"); M5_DUMMY_RETURN };


virtual Fault execute(CheckerCPU *xc, Trace::InstRecord *traceData) const = 0;
virtual Fault eaComp(CheckerCPU *xc, Trace::InstRecord *traceData) const
{ panic("eaComp not defined!"); M5_DUMMY_RETURN };
virtual Fault initiateAcc(CheckerCPU *xc, Trace::InstRecord *traceData) const
{ panic("initiateAcc not defined!"); M5_DUMMY_RETURN };
virtual Fault completeAcc(Packet *pkt, CheckerCPU *xc,
                          Trace::InstRecord *traceData) const
{ panic("completeAcc not defined!"); M5_DUMMY_RETURN };


#endif  // __CPU_STATIC_INST_EXEC_SIGS_HH__

