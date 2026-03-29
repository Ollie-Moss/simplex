#!/bin/bash

if [ -z "$1" ]; then
    echo "Error: Missing argument! 
Usage: build.sh <example> or build.sh test"
    exit 1
fi


# Exit on any error
set -e  

mkdir -p build
cd build

TESTS="OFF"

# Allow cmake v3.5 for freetype build
if [[ "$1" -eq "test" ]]; then
    TESTS="ON"
fi

cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DBUILD_EXAMPLES=ON -DBUILD_TESTS=$TESTS ..  
make

if [[ "$1" != "test" ]]; then
    # Run game from project root
    cd examples
    ./$1
    cd ../..
    exit 0
fi

ctest
cd ..
