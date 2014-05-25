from MemObject import MemObject
from m5.params import *

class RRBus(MemObject):
    type = 'RRBus'
    abstract = True
    slave = VectorSlavePort("vector port for connecting masters")
    master = VectorMasterPort("vector port for connecting slaves")
    # Override the default clock
    clock = '1GHz'
    num_pids = Param.Int(2, "number of security domains")
    turn_length = Param.Int(10, "number of cycles per turn length")
    header_cycles = Param.Cycles(1, "cycles of overhead per transaction")
    width = Param.Int(8, "bus width (bytes)")
    block_size = Param.Int(64, "The default block size if not set by " \
                               "any connected module")

    # The default port can be left unconnected, or be used to connect
    # a default slave port
    default = MasterPort("Port for connecting an optional default slave")

    # The default port can be used unconditionally, or based on
    # address range, in which case it may overlap with other
    # ports. The default range is always checked first, thus creating
    # a two-level hierarchical lookup. This is useful e.g. for the PCI
    # bus configuration.
    use_default_range = Param.Bool(False, "Perform address mapping for " \
                                       "the default port")

class RR_NoncoherentBus(RRBus):
    type = 'RR_NoncoherentBus'