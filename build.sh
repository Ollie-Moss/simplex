#!/bin/bash

# Exit on any error
set -e  

mkdir -p build
cd build

# Allow cmake v3.5 for freetype build
cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DBUILD_EXAMPLES=ON -DBUILD_TESTS=OFF ..  
make

# Run game from project root
cd examples
./$1
cd ../..
