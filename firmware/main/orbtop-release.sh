#!/bin/sh
export OBJDUMP=arm-none-eabi-objdump
orbtop --server localhost:6050 --elf-file release-build/bin/main --routines 20 --interval 1000 --cut-after=40
