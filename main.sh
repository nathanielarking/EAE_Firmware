#!/bin/bash

# Exit if the arguments aren't provided.
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <MIN_VOLTAGE> <TEMP_SETPOINT>"
    exit 1
fi

# Get the arguments
MIN_VOLTAGE=$1
TEMP_SETPOINT=$2

if [ ! -d "build" ]; then
    mkdir build
fi

cd build
cmake  ..
cmake --build .

# Run the code
./src/eae-firmware $MIN_VOLTAGE $TEMP_SETPOINT