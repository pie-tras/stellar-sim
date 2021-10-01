#!/bin/sh

rm -rf stellar_sim.*
cd build
rm -rf .ninja_deps .ninja_log CMakeCache.txt CmakeFiles build.ninja cmake_install.cmake
CC=clang cmake -S .. -B . -G "Ninja"
ninja
mv stellar_sim.* ..