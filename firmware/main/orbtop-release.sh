#!/bin/sh
orbtop --server localhost:6050 --elf-file release-build/bin/main --routines 20 --interval 1000 --cut-after=40
#orbtop --server localhost:6050 --elf-file debug-build/bin/main --routines 20 --exceptions --interval 1000 --agg-lines --cut-after=40
