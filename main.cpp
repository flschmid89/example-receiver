#include <iostream>
#include <thread>
#include <connection_lib.hpp>
#include <zmq.hpp>
#include <atomic>
#include <memory>
std::function<std::optional<std::vector<uint8_t>>(cv::Mat image, json meta)> callbackFunction = [](cv::Mat, json meta)
{
    LOG_F(INFO, "%s", meta.dump().c_str());
    return std::nullopt;
};

ZMQHelper *zmqHelper = new ZMQHelper(true);
ZMQConnectInfo socketInfo {"tcp://ucm-imx8m-plus:4000",zmq::socket_type::rep,true,1000,1000};

std::shared_ptr<zmq::socket_t> socket;
std::atomic<bool> interruptBool = false;
int main()
{
    std::thread loopThread = std::thread([]()
                                         { zmqHelper->loopInteractions(socketInfo, socket,
                                                                       interruptBool,
                                                                       callbackFunction); });

    loopThread.join();
    return 0;
}
/**
 * This repository contains a C++ application that uses ZeroMQ for communication.
 * 
 * The main component of the application is the ZMQHelper class, which is used to establish a ZeroMQ REP (reply) socket connection.
 * The connection is established with the address "tcp://ucm-imx8m-plus:4000".
 * 
 * The application runs a loop in a separate thread that handles interactions with the ZeroMQ socket.
 * The interactions are handled by the `loopInteractions` method of the ZMQHelper class.
 * 
 * The `callbackFunction` is used to process incoming messages. In its current state, it logs the metadata of the incoming message and returns no response.
 * 
 * The `interruptBool` atomic boolean is used to signal the interaction loop to stop.
 * 
 * The application is designed to be run in a single-threaded environment, with the main thread waiting for the interaction loop thread to finish before exiting.
 */