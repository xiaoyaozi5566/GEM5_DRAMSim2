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
    cwd = Param.String("", "current work directory");
    traceFile = Param.String("", "output file for trace generation");
