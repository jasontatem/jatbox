#!/bin/bash

#export GCCARGS="-fno-stack-protector"
export GCCARGS=""
export BUILD_DIR="/home/jat/jatbox/build"

cd $BUILD_DIR
rm *.o
gcc $GCCARGS -c ../log/log.c
gcc $GCCARGS -c ../util.c
gcc $GCCARGS -c ../memory.c
gcc $GCCARGS -c ../cpu.c
gcc $GCCARGS -c ../opcodes.c
gcc $GCCARGS -c ../io.c
gcc $GCCARGS -c ../systemarch.c
gcc $GCCARGS -c ../system.c
gcc $GCCARGS -o ../test *.o
