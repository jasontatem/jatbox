#!/bin/bash

rm *.o
gcc -c util.c
gcc -c memory.c
gcc -c cpu.c
gcc -c opcodes.c
gcc -c io.c
gcc -c systemarch.c
gcc -c system.c
gcc -o test *.o
