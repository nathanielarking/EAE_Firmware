#!/bin/bash

# Builds and runs the unit tests.

if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake ..
cmake --build .
./tests/unit_tests
