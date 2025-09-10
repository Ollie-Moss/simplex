#!/bin/bash

# Exit on any error
set -e  

cd build

# Allow cmake v3.5 for freetype build
cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ..  
make

# Run game from project root
./build
cd ..
