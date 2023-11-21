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
