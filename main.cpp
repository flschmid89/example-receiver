#include <iostream>
#include <thread>
#include <connection_lib.hpp>
#include <zmq.hpp>
#include <atomic>
#include <memory>
#include <blobApp_lib.hpp>
#include <generica_app_lib.hpp>

using namespace Opticus;
Opticus::GenericApp blobApp = Opticus::GenericApp();

ZMQHelper *zmqHelper = new ZMQHelper(true);
ZMQConnectInfo socketInfo{"tcp://*:4001", zmq::socket_type::rep, true, 1000, 1000};

std::shared_ptr<zmq::socket_t> socket;
BlobDetector blobDetector;
std::function<std::optional<std::vector<uint8_t>>(cv::Mat, json)> callbackFunction = [](cv::Mat image, json meta)
{

    json blobs = blobDetector.detectBlobs(image, 127);

    json dataformat = zmqHelper->parseMessage(image, nullptr);
    // LOG_F(INFO, "%s", dataformat.dump().c_str());

    cv::Mat flat = image.clone().reshape(1, static_cast<int>(image.total() * image.channels()));
    std::vector<uchar> vec = image.isContinuous() ? flat : flat.clone();
    json message = {{"subject", "result"}, {"json", blobs}, {"dataformat", dataformat}};
    message["data"] = nlohmann::json::binary(vec);

    // Log the first hundred bytes of vec
    std::ostringstream oss;
    for (size_t i = 0; i < std::min<size_t>(100, vec.size()); ++i)
    {
        oss << static_cast<int>(vec[i]) << ' ';
    }
    LOG_F(INFO, "First 100 bytes: %s", oss.str().c_str());

    return json::to_msgpack(message);
};
std::function<void(BaseControl)> callbackUpdateControl = [](BaseControl control)
{
    LOG_F(INFO, "Updated control %s", control.name.c_str());
    if (control.name == "threshold")
    {
        blobDetector.setThreshold(blobApp.inputs.controlInt[0].value);
    }
};

std::atomic<bool> interruptBool = false;
int main()
{

    blobApp.description = "Blob detection";
    blobApp.name = "Blob detection";
    blobApp.version = "0.0.1";
    blobApp.callbackUpdateControl = std::make_unique<std::function<void(BaseControl)>>(callbackUpdateControl);

    blobApp.inputs.controlInt.push_back(Opticus::ControlInt{
        "threshold",

        "Threshold for binarization",
        Opticus::ParameterType::INTEGER,

        false,
        false,
        1,
        Opticus::DisplayLevel::BEGINNER,
        true,
        127,
        127,
        0,
        255,
        1,
        {}, NULL});

    blobApp.outputs.controlJSON.push_back(Opticus::Control<json>{
        "Blobs",

        "Blobs",
        Opticus::ParameterType::JSON,

        false,
        false,
        1,
        Opticus::DisplayLevel::BEGINNER,
        true,
        127,
        127,
        0,
        255,
        1,
        {}, NULL});
    LOG_F(INFO, "Starting Blob detection");
    LOG_F(INFO, "Inputs %s", blobApp.convertToJSON().dump().c_str());
    std::thread loopThread = std::thread([]()
                                         { zmqHelper->loopInteractions(socketInfo, socket, interruptBool, callbackFunction, blobApp.callbackJSON); });

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