#pragma once
#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <nlohmann/json.hpp>
#include <openssl/ssl.h>
#include <boost/beast/websocket.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/core.hpp>
#include <string>
#include <functional>
#include "trade_execution.h"  // Include the TradeExecution header for access

namespace beast = boost::beast;
namespace asio = boost::asio;
namespace ssl = asio::ssl;
using tcp = asio::ip::tcp;
using json = nlohmann::json;

class WebSocketHandler {
public:
    // Constructor now includes TradeExecution reference
    WebSocketHandler(const std::string& server_host, const std::string& server_port, const std::string& server_endpoint );

    void connect();
    void subscribe(const std::string& symbol); // Declare the subscribe function
    void onMessage(const std::string& message); // Declare the onMessage function
    void sendMessage(const json& message);
    json readMessage();
    void setMarketDataCallback(std::function<void(const std::string&, const json&)> callback); // Declare the setMarketDataCallback function
    void close();

private:
    asio::io_context io_context_;
    ssl::context ssl_context_;
    tcp::resolver tcp_resolver_;
    beast::websocket::stream<ssl::stream<tcp::socket>> websocket_stream_;
    std::string server_host_;
    std::string server_endpoint_;
    std::function<void(const std::string&, const json&)> market_data_callback_; // Add market_data_callback_ member
    // TradeExecution& trade_execution_;  // Reference to TradeExecution object
};

#endif // WEBSOCKET_HANDLER_H
