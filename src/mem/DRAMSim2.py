#
# Authors: Tao Zhang
#          

from m5.params import *
from DRAMSim2Wrapper import *

class DRAMSim2(DRAMSim2Wrapper):
    type = 'DRAMSim2'
    cpu_clock = Param.Clock("2GHz", "CPU speed")
    finalStatsOnly = Param.Bool(True, "only the final stats print out when it's true")
    deviceConfigFile = Param.String("/home/mdl/tzz106/Simulator/GEM5/gem5_9196/ext/DRAMSim2/ini/DDR3_micron_32M_8B_x8_sg15.ini", "DRAM device-specific configuration file")
    systemConfigFile = Param.String("/home/mdl/tzz106/Simulator/GEM5/gem5_9196/ext/DRAMSim2/system.ini.example", "Memory system configuration file")
    tpTurnLength = Param.String("12", "Turn Length for Timing Partitioning")
    genTrace = Param.Bool(False, "Generate trace for benchmark")
    fixAddr = Param.Bool(False, "Use fixed address mapping")
    cwd = Param.String("", "current work directory");
    traceFile = Param.String("./results/DRAMSim.trc", "output file for trace generation");
    outputFile = Param.String("./results/DRAMSim.out", "output file containing DRAMSim results");
    diffPeriod = Param.Bool(False, "Use different periods for different threads")
    p0Period = Param.Int(64, "Period for thread 0")
    p1Period = Param.Int(64, "Period for thread 1")
    offset = Param.Int(0, "offset for turn length")
