#!/bin/bash


# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
  mkdir build
fi

# Navigate to build directory
cd build

# Run CMake to configure the project
cmake -DCMAKE_TOOLCHAIN_FILE=/home/ubuntu/WebSocket_HFT/vcpkg/scripts/buildsystems/vcpkg.cmake ..

# Build the project in debug configuration
cmake --build . --config debug

# Run the executable
./deribit_trader
