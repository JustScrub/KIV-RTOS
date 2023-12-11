#!/bin/bash
rm -r build >/dev/null 2>&1
mkdir -p build >/dev/null 2>&1
cd build

if [ $# -eq 0 ]; then
    cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="../misc/cmake/toolchain-arm-none-eabi-rpi0.cmake" ..
else
    cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="../misc/cmake/toolchain-arm-none-eabi-rpi0.cmake" -DBUILD_STDLIB_ONLY=1 ..
fi

cmake --build . --parallel
#make
#make VERBOSE=1