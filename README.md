# Deribit Trading System

This project is a high-performance trading application designed to interact with the Deribit cryptocurrency exchange. It leverages WebSockets for real-time data streaming and provides a command-line interface for executing various trading operations.

## Key Features

- **Real-time WebSocket Communication**: Establishes a secure WebSocket connection for live market data and order execution.
- **Comprehensive Order Management**: Supports placing, modifying, and canceling orders with ease.
- **Performance Optimization**: Utilizes asynchronous operations and efficient memory management for low-latency performance.
- **Latency Measurement**: Includes tools for measuring and logging the latency of critical operations.

## Project Structure

The project is organized into several modules, each responsible for a specific aspect of the trading system:

- **Main Trading Loop**: `deribit_trader.cpp` - Manages user interactions and integrates all modules.
- **WebSocket Management**: `websocket_handler.h/cpp` - Handles WebSocket connections and message processing.
- **Trade Execution**: `trade_execution.h/cpp` - Manages authentication, order execution, and market data handling.
- **Latency Measurement**: `latency_module.h/cpp` - Provides utilities for measuring and logging operation latency.

## Installation and Setup

### Prerequisites

- **C++17 Compiler**: Ensure you have a compatible compiler installed.
- **CMake**: Version 3.x or higher is required for building the project.
- **Boost Libraries**: Necessary for network communication.
- **OpenSSL**: Required for secure WebSocket connections.

### Installation Steps

1. **Clone the Current Repository**:
   ```bash
   git clone https://github.com/automatesolutions/WebSocket_HFT.git
   cd WebSocket_HFT
   ```

2. **Clone the vcpkg Repository**:
   ```bash
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   ```

3. **Install Required Packages**:
   - Install `websocketpp` and `nlohmann-json` using vcpkg:
     ```bash
     ./vcpkg install websocketpp nlohmann-json
     ```

4. **Integrate vcpkg**:
   ```bash
   ./vcpkg integrate install
   ```

5. **Build the Project**:
   - Return to the root directory and build the project:
     ```bash
     cd ..
     ./build.sh
     ```

## Usage

Upon launching the application, you will be presented with a menu to perform various trading operations. Follow the prompts to place, modify, or cancel orders, and to view market data.










