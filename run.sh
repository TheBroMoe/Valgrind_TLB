#!/bin/bash
make clean -C ./src/
make -C ./src/
clear

# INPUT ARGUMENTS
exe="ex_seg"
pgSize="16"
tlbSize="2"
policy="FIFO"
iFlag="-i"
flush="-f 1000"
# RUNNING valtb379 with valgrind
valgrind --tool=lackey --trace-mem=yes ./src/$exe.out 2>&1 | ./src/valtb379.out $iFlag $flush -p $policy $pgSize $tlbSize
