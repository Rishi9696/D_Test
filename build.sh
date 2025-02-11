#!/bin/bash

# Check if the toolchain file exists
TOOLCHAIN_FILE="/home/ubuntu/goQuant/vcpkg/scripts/buildsystems/vcpkg.cmake"
if [ ! -f "$TOOLCHAIN_FILE" ]; then
  echo "Toolchain file not found: $TOOLCHAIN_FILE"
  echo "Please ensure vcpkg is installed and the toolchain file exists."
  exit 1
fi

# Install necessary build tools if not already installed
if ! command -v cmake &> /dev/null; then
  echo "CMake not found. Installing CMake..."
  sudo apt-get update
  sudo apt-get install -y cmake
fi

if ! command -v make &> /dev/null; then
  echo "Make not found. Installing Make..."
  sudo apt-get update
  sudo apt-get install -y build-essential
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
  mkdir build
fi

# Navigate to build directory
cd build

# Run CMake to configure the project
cmake -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" ..

# Build the project in debug configuration
cmake --build . --config debug

# Run the executable
if [ -f "./deribit_trader" ]; then
  ./deribit_trader
else
  echo "Executable not found: ./deribit_trader"
  exit 1
fi