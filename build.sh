#!/bin/bash

export GCCARGS="-fno-stack-protector"

rm *.o
gcc $GCCARGS -c util.c
gcc $GCCARGS -c memory.c
gcc $GCCARGS -c cpu.c
gcc $GCCARGS -c opcodes.c
gcc $GCCARGS -c io.c
gcc $GCCARGS -c systemarch.c
gcc $GCCARGS -c system.c
gcc $GCCARGS -o test *.o
