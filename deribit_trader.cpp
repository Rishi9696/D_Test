#include "api_credentials.h"
#include "websocket_handler.h"
#include "trade_execution.h"
#include "latency_module.h"
#include <iostream>
#include <string>
#include <exception>
#include <memory>
#include <unordered_map>
#include <future>
#include <vector>
#include <thread>

void executeTrades() {
    try {
        // Initialize WebSocket connection
        WebSocketHandler websocket_handler("test.deribit.com", "443", "/ws/api/v2");
        websocket_handler.connect();

        // Initialize trading operations
        // std::unique_ptr has minimal overhead and is generally faster than (optimization)
        // manual memory management with new and delete.
        auto trade_execution = std::make_unique<TradeExecution>(websocket_handler);

        // Authenticate
        json auth_response = trade_execution->authenticate(CLIENT_ID, CLIENT_SECRET);
        std::cout << "Auth Response: " << auth_response.dump(4) << std::endl;

        // Use unordered_map to cache order responses (optimization)
        // Replace linear search structures with 
        // std::unordered_map for faster access
        std::unordered_map<std::string, json> order_cache;

        while (true) {
            std::string instrument_name, order_id;
            double amount, price;

            // Display menu to the user
            std::cout << "\n--- Trading Menu ---\n";
            std::cout << "1. Place Order\n";
            std::cout << "2. Cancel Order\n";
            std::cout << "3. Modify Order\n";
            std::cout << "4. Get Order Book\n";
            std::cout << "5. View Current Positions\n";
            std::cout << "6. Subscribe to Market Data\n";
            std::cout << "7. Exit\n";
            std::cout << "Enter your choice: ";
            int user_choice;
            std::cin >> user_choice;
            
            auto loop_start = LatencyModule::start();  // Start the timer for end-to-end latency
            
            if (user_choice == 7) {
                std::cout << "Exiting trading application.\n";
                break;
            }

            switch (user_choice) {
            case 1: {  // Place Order
                std::cout << "Enter instrument name (e.g., BTC-PERPETUAL): ";
                std::cin >> instrument_name;
                std::cout << "Enter count: ";
                std::cin >> amount;
                std::cout << "Enter price: ";
                std::cin >> price;

                try {
                    auto order_future = std::async(std::launch::async, [&]() {
                        auto order_start = LatencyModule::start();
                        json buy_response = trade_execution->placeBuyOrder(instrument_name, amount, price);
                        LatencyModule::end(order_start, "Order Placement");
                        return buy_response;
                    });

                    //Optimization
                    // Wait for the order result asynchronously (non-blocking until we get the result)
                    //used std::async for the functions placeBuyOrder, cancelOrder, and modifyOrder to 
                    // make these network requests asynchronous. This allows the application to continue 
                    // running while waiting for responses from the server. This reduces the time spent 
                    // in blocking I/O operations.

                    //The use of std::async enables parallel execution of tasks like placing and canceling 
                    // orders. This means that the main thread can continue interacting with the user 
                    // while the network operations are handled in the background.
                    json buy_response = order_future.get();
                    std::cout << "Order Response: " << buy_response.dump(4) << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error placing order: " << e.what() << std::endl;
                }
                break;
            }

            case 2: {  // Cancel Order
                std::cout << "Enter order ID to cancel: ";
                std::cin >> order_id;

                try {
                        auto cancel_future = std::async(std::launch::async, [&]() {
                        auto cancel_start = LatencyModule::start();
                        json cancel_response = trade_execution->cancelOrder(order_id);
                        LatencyModule::end(cancel_start, "Cancel Order");
                        return cancel_response;
                        }); 

                    json cancel_response = cancel_future.get();
                    std::cout << "Cancel Response: " << cancel_response.dump(4) << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error cancelling order: " << e.what() << std::endl;
                }
                break;
            }

            case 3: {  // Modify Order
                std::cout << "Enter order ID to modify: ";
                std::cin >> order_id;
                std::cout << "Enter new price: ";
                std::cin >> price;
                std::cout << "Enter new amount: ";
                std::cin >> amount;

                try {
                        auto modify_future = std::async(std::launch::async, [&]() {
                        auto modify_start = LatencyModule::start();
                        json modify_response = trade_execution->modifyOrder(order_id, price, amount);
                        LatencyModule::end(modify_start, "Modify Order");
                        return modify_response;
                        });

                    json modify_response = modify_future.get();
                    std::cout << "Modify Response: " << modify_response.dump(4) << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error modifying order: " << e.what() << std::endl;
                }
                break;
            }

            case 4: {  // Get Order Book
                std::cout << "Enter instrument name to view order book (e.g., BTC-PERPETUAL): ";
                std::cin >> instrument_name;

                try {
                    auto order_book_start = LatencyModule::start();
                    json order_book = trade_execution->getOrderBook(instrument_name);
                    LatencyModule::end(order_book_start, "Order Book Fetch");
                    std::cout << "Order Book: " << order_book.dump(4) << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error fetching order book: " << e.what() << std::endl;
                }
                break;
            }

            case 5: {  // View Current Positions
                try {
                    json positions = trade_execution->getPositions();
                    std::cout << "Current Positions: " << positions.dump(4) << std::endl;
                }
                catch (const std::exception& e) {
                    std::cerr << "Error fetching positions: " << e.what() << std::endl;
                }
                break;
            }

            case 6: {  // Subscribe to Market Data
                std::cout << "Enter instrument name to subscribe (e.g., BTC-PERPETUAL): ";
                std::cin >> instrument_name;

                try {
                    trade_execution->subscribeToMarketData(instrument_name);
                    std::cout << "Subscribed to symbol: " << instrument_name << std::endl;

                    // Continuously read and process incoming messages
                    while (true) {
                        websocket_handler.readMessage();
                    }
                }
                catch (const std::exception& e) {
                    std::cerr << "Error subscribing to market data: " << e.what() << std::endl;
                }
                break;
            }

            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
            }
            LatencyModule::end(loop_start, "End-to-End Trading Loop Latency");  // Measure latency for the trading loop
        }

        // Close connection
        websocket_handler.close();

    }
    catch (const std::exception& e) {
        std::cerr << "Error in executeTrades: " << e.what() << std::endl;
    }
}

int main() {
    try {
        executeTrades();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
