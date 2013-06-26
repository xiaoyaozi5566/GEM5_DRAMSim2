from m5.params import *
from m5.SimObject import SimObject
from Controller import RubyController

class DMA_Controller(RubyController):
    type = 'DMA_Controller'
    dma_sequencer = Param.DMASequencer("")
    request_latency = Param.Int(6, "")
