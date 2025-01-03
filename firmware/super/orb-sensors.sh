#!/bin/sh
orbcat --channel "0,%c" -p OFLOW --server localhost:6060 %c 2>&1 | socat - TCP4-LISTEN:6061,reuseaddr,fork
