#!/bin/bash -ev

if [ ! -d tmp ]; then
   mkdir tmp
fi

./fasm kernel.asm kernel.mnt

sudo mount floppy.img tmp
sudo cp kernel.mnt tmp/
sudo umount tmp
