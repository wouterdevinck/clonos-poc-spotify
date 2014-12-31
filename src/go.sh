#!/bin/bash

# Add toolchain to PATH
# This toolchain was built using crosstool-ng, it is one of the included samples
type arm-unknown-linux-uclibcgnueabi-gcc >/dev/null 2>&1 || { export PATH="${PATH}:${HOME}/x-tools/arm-unknown-linux-uclibcgnueabi/bin"; }

# Cross compile for ARM
arm-unknown-linux-uclibcgnueabi-gcc -L`pwd`/../getlib/lib -o run main.c spotify.h -lspotify_embedded_shared -lm

# Run in QEMU 
export LD_LIBRARY_PATH=`pwd`/../getlib/lib:$LD_LIBRARY_PATH
qemu-arm -L ${HOME}/x-tools/arm-unknown-linux-uclibcgnueabi/arm-unknown-linux-uclibcgnueabi run
