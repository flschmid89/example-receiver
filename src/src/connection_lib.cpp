#include <chrono>
#include <connection_lib.hpp>
#include <ctime>
#include <exception>
#include <filesystem>
#include <future>
#include <iomanip>
#include <iostream>
#include <loguru.hpp>
#include <mutex>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <zmq.hpp>
#include <zmq_addon.hpp>
using json = nlohmann::json;
using namespace std::chrono_literals;
namespace fs = std::filesystem;

ZMQHelper::ZMQHelper(bool sendMsgPack_) : sendMsgPack(sendMsgPack_)
{
    context = std::make_unique<zmq::context_t>(1);
}

zmq::message_t ZMQHelper::sendJSONWithSubject(std::shared_ptr<zmq::socket_t> &socket,
                                              std::string subject,
                                              json jsonData,
                                              ZMQConnectInfo &zmqci)
{

    try
    {
        zmq::send_result_t result;
        std::unique_lock<std::mutex> lock(mtx);
        if (sendMsgPack)
        {
            json message = {{subject, jsonData}};
            result = socket->send(zmq::buffer(nlohmann::json::to_msgpack(message)));
        }
        else
        {
            socket->send(zmq::buffer(subject), zmq::send_flags::sndmore);
            result = socket->send(zmq::buffer(jsonData.dump()), zmq::send_flags::none);
            LOG_F(3, "%s", jsonData.dump().c_str());
        }

        if (!result)
            LOG_F(ERROR, "timeout");
        VLOG_F(LOGLEVELANSL, "Send result: %lu", result.value());
        zmq::message_t reply{};
        auto canReceive = canSendResult(socket.get());
        LOG_F(3, "Can receive %s", canReceive ? "true" : "false");
        if (canReceive)
        {
            LOG_F(INFO, "RECEIVE");
            result = socket->recv(reply, zmq::recv_flags::none);
            if (result.has_value() || result.value() > 0)
            {
                LOG_F(INFO, "Receiving  %.*s\n", static_cast<int>(reply.size()), static_cast<char *>(reply.data()));
            }
        }
        lock.unlock();

        return reply;
    }
    catch (const zmq::error_t &e)
    {
        LOG_F(ERROR, "%s", e.what());
        socket = startSocket(zmqci);

        return {};
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s", e.what());

        return {};
    }
}

zmq::message_t ZMQHelper::sendJSON(std::shared_ptr<zmq::socket_t> &socket, json jsonData, ZMQConnectInfo &zmqci)
{

    try
    {
        std::unique_lock<std::mutex> lock(mtx);
        zmq::send_result_t result;
        if (sendMsgPack)
        {
            result = socket->send(zmq::buffer(nlohmann::json::to_msgpack(jsonData)));
        }
        else
        {
            result = socket->send(zmq::buffer(jsonData.dump()), zmq::send_flags::none);
        }
        LOG_F(INFO, "%s", jsonData.dump().c_str());
        if (!result)
            LOG_F(ERROR, "timeout");
        VLOG_F(LOGLEVELANSL, "Send result: %lu", result.value());
        zmq::message_t reply{};
        auto canReceive = canSendResult(socket.get());
        if (canReceive)
        {
            LOG_F(3, "RECEIVE");
            result = socket->recv(reply, zmq::recv_flags::none);
            if (result.has_value() || result.value() > 0)
            {
                LOG_F(1, "Receiving  %zu", reply.size());
            }
        }
        return reply;
    }

    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s for %s", e.what(), zmqci.socketString.c_str());
        socket = startSocket(zmqci);

        return {};
    }
    // wait for reply from server
}

json ZMQHelper::parseMessage(const cv::Mat &image, const json &j) const
{
    std::chrono::milliseconds ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    json dataformat;
    if (j != nullptr)
        dataformat["meta"] = j;
    if (j.contains("meta"))
    {
        if (j["meta"].is_array() || j["meta"].is_object())
        {
            dataformat["meta"].insert(j["meta"].begin(), j["meta"].end());
        }
        if (dataformat["meta"].is_object() && dataformat["meta"].contains("meta"))

            dataformat["meta"].erase("meta");
    }

    dataformat["cols"] = image.cols;
    dataformat["rows"] = image.rows;
    dataformat["channels"] = image.channels();
    dataformat["type"] = "raw";
    dataformat["timestamp"] = ms.count();
    VLOG_F(LOGLEVELANSL, "%s", dataformat.dump().c_str());
    return dataformat;
}
zmq::send_result_t ZMQHelper::zmqSendImage(std::shared_ptr<zmq::socket_t> &socket,
                                           const cv::Mat &image,
                                           const json &metadata,
                                           const std::string &subject)
{

    json dataformat = parseMessage(image, metadata);

    size_t frameSize =
        static_cast<size_t>(image.rows) * static_cast<size_t>(image.cols) * static_cast<size_t>(image.channels());
    VLOG_F(LOGLEVELANSL, "Sending size %zu ", frameSize);
    zmq::send_result_t result;
    std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);
    if (sendMsgPack)
    {
        cv::Mat flat = image.reshape(1, static_cast<int>(image.total() * image.channels()));
        std::vector<uchar> vec = image.isContinuous() ? flat : flat.clone();
        json message = {{"subject", subject}, {"json", metadata}, {"dataformat", dataformat}};
        message["data"] = nlohmann::json::binary(vec);
        result = socket->send(zmq::buffer(nlohmann::json::to_msgpack(message)));
    }
    else
    {
        if (!subject.empty())
        {
            socket->send(zmq::buffer(subject), zmq::send_flags::sndmore);
        }

        socket->send(zmq::buffer(static_cast<void *>(image.data), frameSize), zmq::send_flags::sndmore);
        result = socket->send(zmq::buffer(dataformat.dump()), zmq::send_flags::none);
    }

    if (!result)
        LOG_F(ERROR, "timeout");
    VLOG_F(LOGLEVELANSL, "Image sent (%d,%d) Send result: %zu ", image.cols, image.rows, result.value());
    return result;
}

zmq::message_t ZMQHelper::sendImage(std::shared_ptr<zmq::socket_t> &socket,
                                    const cv::Mat &image,
                                    std::string socketString,
                                    const json &metadata,
                                    const std::string &subject,
                                    bool bind)
{

    try
    {
        std::unique_lock<std::mutex> lock(mtx, std::adopt_lock);

        zmqSendImage(socket, image, metadata, subject);

        zmq::message_t reply{};
        auto canReceive = canSendResult(socket.get());
        LOG_F(3, "Can receive %s", canReceive ? "true" : "false");
        if (canReceive)
        {
            auto result = socket->recv(reply, zmq::recv_flags::none);

            if (!result.has_value() || result.value() <= 0)
            {
                LOG_F(ERROR, "Receiving error for socket %s, error: %lu", socketString.c_str(), result.value());
            }
        }

        return reply;
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "[%s]: %s", socketString.c_str(), e.what());
        mtx.unlock();
        socket = startSocket(socketString, static_cast<zmq::socket_type>(socket->get(zmq::sockopt::type)), bind);
        return {};
    }
    // wait for reply from server
}

json ZMQHelper::sendImageWithJSONResult(std::shared_ptr<zmq::socket_t> &socket,
                                        const cv::Mat &image,
                                        std::string socketString,
                                        const json &metadata,
                                        bool bind)
{

    std::unique_lock<std::mutex> lock(mtx);

    auto response = sendImage(socket, image, socketString, metadata, "", bind);
    if (response.size() > 0)
    {
        try
        {
            json jResponse = nlohmann::json::parse(response.to_string());
            return jResponse;
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "Parsing error %s for: %s ", response.to_string().c_str(), e.what());
        }
    }

    else
    {
        LOG_F(WARNING, "No data or timeout for socket %s", socketString.c_str());
    }
    return json();
}
cv::Mat ZMQHelper::sendImageWithResultImage(std::shared_ptr<zmq::socket_t> &socket,
                                            const cv::Mat &image,
                                            std::string socketString,
                                            const json &metadata,
                                            bool bind)
{

    cv::Mat resultImage;

    try
    {
        std::unique_lock<std::mutex> lock(mtx);

        zmqSendImage(socket, image, metadata);

        zmq::message_t reply{};
        if (!socket->recv(reply, zmq::recv_flags::none).has_value())
            throw std::exception();

        size_t frameSizeResult = static_cast<size_t>(image.rows) * static_cast<size_t>(image.cols) * 3;
        if (reply.size() == frameSizeResult)
        {
            resultImage = cv::Mat(image.rows, image.cols, CV_8UC3, reply.data());
            return resultImage.clone();
            // storeImage(resultImage, "/test/cpp");
        }

        else
        {
            LOG_F(ERROR, "Image size different: %zu, is %zu", frameSizeResult, reply.size());
        }
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s", e.what());
        socket = startSocket(socketString, static_cast<zmq::socket_type>(socket->get(zmq::sockopt::type)), bind);
    }
    resultImage = cv::Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(0, 0, 255));
    return resultImage;
    // wait for reply from server
}
cv::Mat ZMQHelper::getInference(cv::Mat image, const ZMQConnectInfo &zmqci)
{
    cv::Mat resultImage;

    try
    {
        std::cout << "Starting inference" << std::endl;
        auto inference = sendImageWithResultImage(sinkSocket, image, zmqci.socketString, {}, zmqci.bind);

        resultImage = inference;
        std::cout << std::endl
                  << "Size: " << inference.size() << std::endl;
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s", e.what());
    }

    return resultImage;
}
int ZMQHelper::getInferenceWithResult(cv::Mat image,
                                      std::shared_ptr<zmq::socket_t> &socket,
                                      const ZMQConnectInfo &zmqci,
                                      json &meta)
{
    int result = -1;
    try
    {
        std::cout << "Starting inference" << std::endl;
        auto inference = sendImage(socket, image, zmqci.socketString, meta, "", zmqci.bind);

        result = std::atoi(static_cast<char *>(inference.data()));
        std::cout << std::endl
                  << "Result: " << result << std::endl;
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s", e.what());
    }

    return result;
}
std::shared_ptr<zmq::socket_t> ZMQHelper::startSocket(const std::string port,
                                                      const zmq::socket_type type,
                                                      const bool bind)
{
    std::shared_ptr<zmq::socket_t> socket;

    try
    {
        VLOG_F(LOGLEVELANSL, "Starting socket %s", port.c_str());

        socket = std::make_shared<zmq::socket_t>(*context, type);
        if (bind)
            socket->bind(port);
        else
            socket->connect(port);
        switch (type)
        {
        case zmq::socket_type::pub:
            socket->set(zmq::sockopt::sndtimeo, SENDTIMEOUT);

            break;
        case zmq::socket_type::sub:
            socket->set(zmq::sockopt::subscribe, "");
            socket->set(zmq::sockopt::rcvtimeo, RECVTIMEOUT);
            break;
        case zmq::socket_type::req:
            socket->set(zmq::sockopt::rcvtimeo, RECVTIMEOUT);
            socket->set(zmq::sockopt::sndtimeo, SENDTIMEOUT);
            socket->set(zmq::sockopt::linger, 0);
            break;
        case zmq::socket_type::rep:
            socket->set(zmq::sockopt::rcvtimeo, 60000);
            socket->set(zmq::sockopt::sndtimeo, 60000);
            socket->set(zmq::sockopt::linger, 0);
            break;

        default:
            break;
        }
        return socket;
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s for: %s", e.what(), port.c_str());
    }

    return socket;
}
std::shared_ptr<zmq::socket_t> ZMQHelper::startSocket(const ZMQConnectInfo &zmqci)
{
    return startSocket(zmqci.socketString, zmqci.socketType, zmqci.bind);
}

std::shared_ptr<connect_monitor_t> ZMQHelper::addMonitor(zmq::socket_t *socket, std::string transportProtocol)
{
    // const int events = ZMQ_EVENT_CONNECTED;
    std::shared_ptr<connect_monitor_t> monitor(nullptr);
    // Monitor sock using the given transport for internal communication
    std::string protocolString = "ipc://conmon";
    if (transportProtocol == "ipc")
        protocolString = "ipc://conmon";
    else if (transportProtocol == "inproc")
        protocolString = "ipc://conmon";
    else if (transportProtocol == "tcp")
        protocolString = "tcp://conmon";
    else
    {
        LOG_F(ERROR, "transportProtocol %s is not defined!", transportProtocol.c_str());
        return monitor;
    }

    // // mon runs forever....

    // Now, try it a different way:
    monitor = std::make_shared<connect_monitor_t>();
    // here, default is ZMQ_EVENT_ALL
    monitor->init(*socket, "inproc://conmon2");
    // init returns and we poll when we want
    if (!monitor->check_event(100))
    {
        std::cout << "timeout" << std::endl;
    }
    return monitor;
}
int ZMQHelper::getChannelsSize(const int &rawType)
{
    int imageType = CV_8UC1;
    switch (rawType)
    {
    case 1:
        imageType = CV_8UC1;

        break;
    case 3:
        imageType = CV_8UC3;

        break;
    case 4:
        imageType = CV_8UC4;

        break;

    default:

        break;
    }
    return imageType;
}
bool ZMQHelper::startLoop(std::shared_ptr<zmq::socket_t> &socket, const ZMQConnectInfo &zmqci)
{
    std::string threadName =
        "S_" + zmqci.socketString.substr(std::max(zmqci.socketString.size() - 10, static_cast<size_t>(0)));

    loguru::set_thread_name(threadName.c_str());

    while (!socket || socket->handle() == nullptr)
    {
        socket = startSocket(zmqci);
    }
    LOG_F(INFO, "Starting loop for %s (%s)", zmqci.socketString.c_str(), threadName.c_str());

    return canSendResult(socket.get());
}
std::optional<std::tuple<bool, cv::Mat, json>> ZMQHelper::castMessageToImage(std::vector<zmq::message_t> &recv_msgs)
{
    json dataformat = json::parse(recv_msgs[1].to_string());
    cv::Mat image;

    if (!(dataformat.contains("channels") && dataformat.contains("rows") && dataformat.contains("cols")))
    {
        LOG_F(WARNING, "No dataformat");
        return std::nullopt;
    }

    int imageType = getChannelsSize(dataformat.contains("channels") ? static_cast<int>(dataformat["channels"]) : 3);
    size_t frameSize = static_cast<size_t>(dataformat["rows"]) * static_cast<size_t>(dataformat["cols"]) *
                       static_cast<size_t>(dataformat["channels"]);
    if (frameSize == recv_msgs[0].size())
    {
        return std::tuple<bool, cv::Mat, json>{
            dataformat.contains("info"),
            cv::Mat(dataformat["rows"], dataformat["cols"], imageType, recv_msgs[0].data()),
            dataformat};
    }
    else
    {

        LOG_F(ERROR, "Different sizes of definition and data: %zu --> %zu", frameSize, recv_msgs[0].size());
        return std::nullopt;
    }
}

cv::Mat ZMQHelper::castMessageToImage(zmq::message_t &recv_msgs, bool &hasData, json &jMeta)
{
    jMeta = json::from_msgpack(recv_msgs.to_string());
    cv::Mat image;
    hasData = false;
    if (!jMeta.contains("dataformat"))
    {
        LOG_F(WARNING, "No dataformat");
        return image;
    }
    json dataformat = jMeta["dataformat"];

    if (!(dataformat.contains("channels") && dataformat.contains("rows") && dataformat.contains("cols")))
    {
        hasData = false;
        LOG_F(WARNING, "No dataformat");
        return image;
    }
    LOG_F(INFO, "Dataformat %s", dataformat.dump().c_str());

    int imageType = getChannelsSize(dataformat.contains("channels") ? static_cast<int>(dataformat["channels"]) : 3);
    size_t frameSize = static_cast<size_t>(dataformat["rows"]) * static_cast<size_t>(dataformat["cols"]) *
                       static_cast<size_t>(dataformat["channels"]);

    if (frameSize != jMeta["data"].get<json::binary_t>().size())
    {
        LOG_F(ERROR, "Different sizes of definition and data: %zu --> %zu", frameSize, jMeta["data"].get<json::binary_t>().size());
        return image;
    }
    std::vector<uint8_t> vec = jMeta["data"].get<json::binary_t>();
    cv::Mat resultImage = cv::Mat(dataformat["rows"], dataformat["cols"], imageType, (void *)vec.data());
    hasData = true;
    // jMeta.erase("data");
    return std::move(resultImage);
}

void ZMQHelper::loopInteractions(const ZMQConnectInfo &zmqci,
                                 std::shared_ptr<zmq::socket_t> &internsocket,
                                 std::atomic<bool> &interruptBool,
                                 std::function<std::optional<std::vector<uint8_t>>(cv::Mat image, json meta)> fn,
                                 std::function<std::optional<std::vector<uint8_t>>(json)> fnJson)
{
    bool sendResult = startLoop(internsocket, zmqci);
    zmq::message_t recv_msg;

    while (!interruptBool)
    {
        std::optional<std::vector<uint8_t>> fnResult;
        std::optional<std::tuple<bool, cv::Mat, json>> castResult;
        bool hasData = false;
        cv::Mat image;
        json jInformation;
        try
        {
            if (sendMsgPack)
            {
                zmq::recv_result_t result = internsocket->recv(recv_msg);
                if (!result)
                    continue;
                image = castMessageToImage(recv_msg, hasData, jInformation);
            }

            // else
            // {
            //     std::vector<zmq::message_t> recv_msgs;

            //     zmq::recv_result_t result = zmq::recv_multipart(*internsocket, std::back_inserter(recv_msgs));
            //     if (!result)
            //         continue;

            //     image = castMessageToImage(recv_msgs, hasData);
            //     // recv_msg = recv_msgs[1];
            // }

            if (!hasData)
            {

               
                    if (fnJson != nullptr)
                        fnResult = fnJson(jInformation);
                }
                else
                {
                    fnResult = fn(image, jInformation);
                }
            // }
            // else if (fnJson != nullptr)
            // {
            //     json jInformation = json::from_msgpack(recv_msg.to_string());
            //     fnResult = fnJson(jInformation);
            // }

            if (sendResult)
            {
                if (fnResult.has_value())
                    internsocket->send(zmq::buffer(fnResult.value()), zmq::send_flags::none);
                else
                    internsocket->send(zmq::buffer("Received"), zmq::send_flags::none);
            }
            // std::this_thread::sleep_for(100ms);
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "%s", e.what());
            internsocket->close();
            std::this_thread::sleep_for(500ms);
            internsocket = startSocket(zmqci);
            // throw std::exception();
        }
    }
}

void ZMQHelper::loopInteractionsJSON(ZMQConnectInfo zmqci,
                                     std::shared_ptr<zmq::socket_t> internsocket,
                                     std::string subject,
                                     std::atomic<bool> &interruptBool,
                                     std::function<std::optional<std::vector<uint8_t>>(json meta)> fn)
{

    bool sendResult = startLoop(internsocket, zmqci);

    while (!interruptBool)
    {
        try
        {
            zmq::message_t recv_msgs;

            if (!subject.empty())
            {
                std::vector<zmq::message_t> recv_vector;
                zmq::recv_result_t result = zmq::recv_multipart(*internsocket, std::back_inserter(recv_vector));
                if (!result)
                    continue;
                if (recv_vector[0].to_string() != subject)
                    continue;
                recv_msgs.copy(recv_vector[1]);
            }
            else
            {
                zmq::recv_result_t result = internsocket->recv(recv_msgs);
                if (!result)
                    continue;
                LOG_F(INFO, "Message received");
            }

            json message =
                json::from_msgpack(std::vector<uchar>(static_cast<char *>(recv_msgs.data()),
                                                      static_cast<char *>(recv_msgs.data()) + recv_msgs.size()));
            LOG_F(INFO, "Received %s", message.dump().c_str());

            // std::string jsonRaw = recv_msgs.to_string();
            // json dataformat = json::parse(jsonRaw);
            VLOG_F(LOGLEVELANSL, "Parsed");
            auto result = fn(message);
            if (sendResult)
            {
                if (result.has_value())
                    internsocket->send(zmq::buffer(result.value()), zmq::send_flags::none);
                else
                    internsocket->send(zmq::buffer("Received"), zmq::send_flags::none);
            }
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "%s", e.what());
            internsocket = startSocket(zmqci);
        }
    }
}

bool ZMQHelper::canSendResult(const zmq::socket_t *socket)
{
    bool sendResult = true;
    try
    {
        auto socketType = static_cast<zmq::socket_type>(socket->get(zmq::sockopt::type));
        switch (socketType)
        {
        case zmq::socket_type::sub:
            sendResult = false;
            break;
        case zmq::socket_type::pub:
            sendResult = false;
            break;
        default:
            break;
        }
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s", e.what());
    }
    return sendResult;
}
