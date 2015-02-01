#!/bin/bash

# This file is part of project Clonos - Wouter Devinck
# https://github.com/wouterdevinck/clonos-poc-spotify

# Clean up when receiving SIGINT
trap "killall avahi-publish 2>/dev/null" INT

# Get home-built qemu 2.2 instead of ubuntu qemu 2.0
export PATH=$PATH:/usr/local/bin

# Choose a toolchain - These toolchains were built using crosstool-ng, they are included samples
#  (a) ARM (uclibc)
#GCC=arm-unknown-linux-uclibcgnueabi-gcc
#TOOLPATH="${HOME}/x-tools/arm-unknown-linux-uclibcgnueabi/bin"
#LIBSPATH="${HOME}/x-tools/arm-unknown-linux-uclibcgnueabi/arm-unknown-linux-uclibcgnueabi"
#  (b) ARM (glibc)
GCC=arm-unknown-linux-gnueabi-gcc
TOOLPATH="${HOME}/x-tools/arm-unknown-linux-gnueabi/bin"
LIBSPATH="${HOME}/x-tools/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi"

# Add toolchain to PATH
type ${GCC} >/dev/null 2>&1 || { export PATH="${PATH}:${TOOLPATH}"; }

# Cross compile for ARM
${GCC} -I`pwd`/../httplib/lib/include \
       -L`pwd`/../getlib/lib \
       -L`pwd`/../httplib/lib/lib \
       -o run \
       main.c spotify.h httpserver.c httpserver.h \
       -lspotify_embedded_shared \
       -lmicrohttpd \
       -lm

# Run in QEMU 
export LD_LIBRARY_PATH=`pwd`/../httplib/lib/lib:`pwd`/../getlib/lib:$LD_LIBRARY_PATH
qemu-arm -L ${LIBSPATH} -r 3.15.4 run

# Cleanup
killall avahi-publish 2>/dev/null