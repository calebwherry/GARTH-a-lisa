#!/bin/bash
rm -rf cmake_build/
mkdir cmake_build
cd cmake_build/
cmake ..
make VERBOSE=1
cd ..
./cmake_build/src/garth
exit 0
