#!/usr/bin/bash

build/ARM/gem5.fast \
    --stats-file=stats.txt \
    configs/dramsim2/dramsim2_se.py \
    --cpu-type=detailed \
    --caches \
    --l2cache \
    --l3cache \
    --l3_size=4MB \
    --maxinsts=1000000 \
    --dramsim2 \
    --numpids=2 \
    --tpturnlength=6 \
    --devicecfg=./ext/DRAMSim2/ini/DDR3_micron_16M_8B_x8_sg15.ini  \
    --systemcfg=./ext/DRAMSim2/system_none.ini \
    --outputfile=/dev/null \
    --p0='/home/andrew/Documents/Research/GEM5_DRAMSim2/tests/test-progs/test/arm/randmem -p 10000'\
    --p1=/home/andrew/Documents/Research/GEM5_DRAMSim2/tests/test-progs/test/arm/nothing
