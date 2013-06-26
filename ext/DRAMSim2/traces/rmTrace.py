#!/usr/bin/python2.7

import os

filelist = [ "mase_art_MT_LOW.trc", "mase_art_MT_MEDIUM.trc", "mase_art_MT_HIGH.trc"]
for f in filelist:
    os.remove(f)