#!/bin/bash

set -e  # Exit on any error

# Step into build directory
cd build

# Run CMake and build
cmake ..
make

# Step back to project root
cd ..

# Run the game
./build/GameEngine
