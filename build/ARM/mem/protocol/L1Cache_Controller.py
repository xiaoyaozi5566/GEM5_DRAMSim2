from m5.params import *
from m5.SimObject import SimObject
from Controller import RubyController

class L1Cache_Controller(RubyController):
    type = 'L1Cache_Controller'
    sequencer = Param.RubySequencer("")
    cacheMemory = Param.RubyCache("")
    cache_response_latency = Param.Int(12, "")
    issue_latency = Param.Int(2, "")
    send_evictions = Param.Bool("")
