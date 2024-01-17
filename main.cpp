#include <iostream>
#include <thread>
#include <connection_lib.hpp>
#include <zmq.hpp>
#include <atomic>
#include <memory>
#include <blobApp_lib.hpp>
#include <generica_app_lib.hpp>
#include <codecorp.h>
#include <helper_lib.hpp>
using namespace Opticus;

void castEnvVariables();
Opticus::GenericApp blobApp = Opticus::GenericApp();

ZMQHelper *zmqHelper = new ZMQHelper(true);
ZMQConnectInfo socketInfo{"tcp://*:4001", zmq::socket_type::rep, true, 1000, 1000};
opticus::Helper helper;
std::string licenseCode = "326D2BC480C943F6B46C";
std::shared_ptr<zmq::socket_t> inOutSocket;
BlobDetector blobDetector;

bool renderBoundingBoxes = false;


std::function<std::optional<std::vector<uint8_t>>(cv::Mat, json)> callbackFunction = [](cv::Mat image, json meta)
{

    useEdgeDecoder(image);

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
    // LOG_F(INFO, "First 100 bytes: %s", oss.str().c_str());

    return json::to_msgpack(message);
};
std::function<void(BaseControl)> callbackUpdateControl = [](BaseControl control)
{
    LOG_F(INFO, "Updated control %s", control.name.c_str());
    if (control.name == "threshold")
    {
        blobDetector.setThreshold(blobApp.inputs.controlInt[0].get_value());
    }
};

std::atomic<bool> interruptBool = false;
int main()
{
    castEnvVariables();
    blobApp.description = "Barcode Scanner";
    blobApp.name = "Scanning different types of barcodes live in video stream";
    blobApp.version = "0.0.1";
    blobApp.callbackUpdateControl = std::make_unique<std::function<void(BaseControl)>>(callbackUpdateControl);

  for(auto &decoderAppsSettingsDescription: decoderAppsSettingsDescriptions)
    {
        blobApp.inputs.controlBool[decoderAppsSettingsDescription.name] = (Opticus::CreateBoolControl(
            decoderAppsSettingsDescription.name,
            decoderAppsSettingsDescription.description,
            *decoderAppsSettingsDescription.address));
        blobApp.inputs.controlBool[decoderAppsSettingsDescription.name].connectedValue = std::make_unique<bool>(decoderAppsSettingsDescription.address);
    }
    blobApp.inputs.controlBool["output_image"] = (Opticus::CreateBoolControl(
            "output_image",
            "Render the bounding boxes in the image",
            renderBoundingBoxes));
     blobApp.inputs.controlBool["output_image"].connectedValue = std::make_unique<bool>(&renderBoundingBoxes);

    if (initEdgeDecoder(licenseCode.data()) == -1)
    {
        LOG_F(ERROR, "Failed to initialize EdgeDecoder");
        return -1;
    }

    LOG_F(INFO, "Starting Blob detection");
    LOG_F(INFO, "Inputs %s", blobApp.convertToJSON().dump().c_str());
    std::thread loopThread = std::thread([]()
                                         { zmqHelper->loopInteractions(socketInfo, inOutSocket, interruptBool, callbackFunction, blobApp.callbackJSON); });

    loopThread.join();
    return 0;
}

void castEnvVariables()
{
    const std::string APPNAME = "APP";
    helper.castEnvString(APPNAME+"_SOCKET", socketInfo.socketString);
    helper.castEnvString(APPNAME+"_LICENSECODE", licenseCode);

  

 

}
/**
 * This repository contains a C++ application that uses ZeroMQ for communication.
 *
 * The main component of the application is the ZMQHelper class, which is used to establish a ZeroMQ REP (reply) inOutSocket connection.
 * The connection is established with the address "tcp://ucm-imx8m-plus:4000".
 *
 * The application runs a loop in a separate thread that handles interactions with the ZeroMQ inOutSocket.
 * The interactions are handled by the `loopInteractions` method of the ZMQHelper class.
 *
 * The `callbackFunction` is used to process incoming messages. In its current state, it logs the metadata of the incoming message and returns no response.
 *
 * The `interruptBool` atomic boolean is used to signal the interaction loop to stop.
 *
 * The application is designed to be run in a single-threaded environment, with the main thread waiting for the interaction loop thread to finish before exiting.
 */