#!/bin/bash

echo "Cleaning motorboard project: Start!"

rm -rvf build/

rm -vf Makefile *.make *.elf *.map

echo "Cleaning motorboard project: Done!"
