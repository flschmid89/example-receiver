#pragma once

#include "dispatcher_lib.hpp"
#include <atomic>
#include <cstddef>
#include <loguru.hpp>
#include <memory>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <optional>
#include <string>
#include <thread>
#include <vector>
#include <zmq.hpp>

using json = nlohmann::json;
#include <mutex> // std::mutex

class connect_monitor_t : public zmq::monitor_t
{
public:
    void on_event_connected(const zmq_event_t &event, const char *addr) override
    {
        LOG_F(INFO, "Got connection from %s %d", addr, event.value);
    };
    void on_event_disconnected(const zmq_event_t &event, const char *addr) override
    {
        LOG_F(ERROR, "Disconnection from %s %d", addr, event.value);
    };
};
struct ZMQConnectInfo
{
    std::string socketString = "";
    zmq::socket_type socketType = zmq::socket_type::pub;
    bool bind = false;
    unsigned int sendTimeout = 1000;
    unsigned int receiveTimeout = 1000;
};

class ZMQHelper
{
    const int SENDTIMEOUT = 1000;
    const int RECVTIMEOUT = 5000;


private:
    std::unique_ptr<zmq::context_t> context;
    std::mutex mtx; // mutex for critical section

    bool sendMsgPack = false; /* If true, no multimessage is sent*/

   

    bool startLoop(std::shared_ptr<zmq::socket_t> &socket, const ZMQConnectInfo &zmqci);

    /**
   * @brief Multipart send command for zmq
   * 
   * @param socket 
   * @param image 
   * @param metadata 
   * @return zmq::send_result_t 
   */
    zmq::send_result_t zmqSendImage(std::shared_ptr<zmq::socket_t> &socket,
                                    const cv::Mat &image,
                                    const json &metadata = {},
                                    const std::string &subject = "");

public:
    loguru::Verbosity LOGLEVELANSL = loguru::Verbosity_2;

    /**
   * @brief Checks if both direction communication is possible
   * 
   * @param socket 
   * @return true 
   * @return false 
   */
    bool canSendResult(const zmq::socket_t *socket);
    std::shared_ptr<zmq::socket_t> sourceSocket;
    std::shared_ptr<zmq::socket_t> sinkSocket;
   
    json parseMessage(const cv::Mat &image, const json &j) const;
    ZMQHelper(bool sendMsgPack = false);
    
    zmq::message_t sendImage(std::shared_ptr<zmq::socket_t> &socket,
                             const cv::Mat &image,
                             std::string socketString = "",
                             const json &metadata = {},
                             const std::string &subject = "",
                             bool bind = true
                             );
    cv::Mat sendImageWithResultImage(std::shared_ptr<zmq::socket_t> &socket,
                                     const cv::Mat &image,
                                     std::string socketString,
                                     const json &metadata,
                                     bool bind);

    /**
   * @brief Send JSON Data over zeroMQ, mainly designed for SUB
   *
   * @param socket
   * @param subject
   * @param jsonData
   * @return zmq::message_t
   */
    zmq::message_t sendJSONWithSubject(std::shared_ptr<zmq::socket_t> &socket,
                                       std::string subject,
                                       json jsonData,
                                       ZMQConnectInfo &zmqci);
    zmq::message_t sendJSON(std::shared_ptr<zmq::socket_t> &socket, json jsonData, ZMQConnectInfo &zmqci);
    cv::Mat getInference(cv::Mat image, const ZMQConnectInfo &zmqci);
    json sendImageWithJSONResult(std::shared_ptr<zmq::socket_t> &socket,
                                 const cv::Mat &image,
                                 std::string socketString,
                                 const json &metadata,
                                 bool bind);
    int getInferenceWithResult(cv::Mat image,
                               std::shared_ptr<zmq::socket_t> &socket,
                               const ZMQConnectInfo &zmqci,
                               json &meta);

    std::shared_ptr<zmq::socket_t> startSocket(const ZMQConnectInfo &zmqci);
    std::shared_ptr<zmq::socket_t> startSocket(const std::string port,
                                               const zmq::socket_type type,
                                               const bool bind = false);
    cv::Mat castMessageToImage(zmq::message_t &recv_msgs, bool &hasData, json &jMeta);

    std::optional<std::tuple<bool, cv::Mat, json>> castMessageToImage(std::vector<zmq::message_t> &recv_msgs);
    /**
   * @brief Translates inputs from json
   * 
   * @param rawType 
   * @return int 
   */
    int getChannelsSize(const int &rawType);


    std::shared_ptr<connect_monitor_t> addMonitor(zmq::socket_t *socket, std::string transportProtocol);

    void loopInteractions(const ZMQConnectInfo &zmqci,
                          std::shared_ptr<zmq::socket_t> &internsocket,
                          std::atomic<bool> &interruptBool,
                          std::function<std::optional<std::vector<uint8_t>>(cv::Mat image, json meta)> fn = nullptr,
                        std::function<std::optional<std::vector<uint8_t>>(json)> fnJson = nullptr);


    void loopInteractionsJSON(ZMQConnectInfo zmqci,
                              std::shared_ptr<zmq::socket_t> internsocket,
                              std::string subject,
                              std::atomic<bool> &interruptBool,
                              std::function<std::optional<std::vector<uint8_t>>(json meta)> fn);


    std::function<void(cv::Mat, std::string)> func;
    std::unique_ptr<std::thread> startWatcher(std::string zmqPort);
 



    ~ZMQHelper() = default;
};
