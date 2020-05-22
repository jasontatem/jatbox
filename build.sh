#!/bin/bash

#export GCCARGS="-fno-stack-protector"
export GCCARGS="-std=c99 -Wall -pedantic -Wshadow -Wstrict-aliasing -Wstrict-overflow -O2"
export SDLARGS="-D_REENTRANT -I/usr/include/SDL2 -lSDL2"
export BUILD_DIR="/home/jat/jatbox/build"

cd $BUILD_DIR
rm *.o
gcc $GCCARGS -c ../log/log.c $SDLARGS
gcc $GCCARGS -c ../util.c $SDLARGS
gcc $GCCARGS -c ../display.c $SDLARGS
gcc $GCCARGS -c ../memory.c $SDLARGS
gcc $GCCARGS -c ../cpu.c $SDLARGS
gcc $GCCARGS -c ../opcodes.c $SDLARGS
gcc $GCCARGS -c ../io.c $SDLARGS
gcc $GCCARGS -c ../systemarch.c $SDLARGS
gcc $GCCARGS -c ../system.c $SDLARGS
gcc $GCCARGS -o ../test *.o $SDLARGS
