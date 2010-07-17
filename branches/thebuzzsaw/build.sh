#!/bin/bash
cd ./source/project/
mkdir build
cd ./build
cmake ..
make
cd ../../../
./zero2d


