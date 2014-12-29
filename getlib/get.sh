#!/bin/bash

# Script to get the Spotify Connect ARM library from 
# encrypted firmware, downloaded from the Onkyo website.

# Install tools
sudo apt-get install libc6-dev-i386 
sudo apt-get install squashfs-tools

# Create target and working directory
mkdir lib
mkdir temp
cd temp

# Compile decryption tool (in 32 bit mode)
cp ../onkyo-decryptor.c .
gcc -o onkyo-dec -m32 onkyo-decryptor.c

# Get the firmware, unzip it and decrypt it
wget http://download.onkyo.com/AVR0018/ONKAVR0018_EAEAEAEAEAEAEAEA_109.zip
unzip ONKAVR0018_EAEAEAEAEAEAEAEA_109.zip 
./onkyo-dec
cd extracted/

# Unpack one of the squashfs file systems in the firmware
mkdir fs
sudo unsquashfs -f -d fs of4.DA83XLA_010203040506.05107 
cp fs/onkyo/avr/lib/libspotify_embedded_shared.so ../../lib
cd ../..

# Clean up
sudo rm -r temp

