#pragma once
#include <iostream>
#include <signal.h>

#include <connection_lib.hpp>
#include <loguru.hpp>
#include <mutex>
#include <string>
#include <string_view>
#include <tuple>
#include <iomanip>
#include <memory>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace opticus
{


  /**
   * @brief Setup for loguru logger and file sink for warnings and errors
   *
   * @param argc Incoming argc from application
   * @param argv Incoming argv from application
   * @param filePathError Filepath with name for file for logging, if empty, file
   * is not written
   * @param warningPathError  Filepath with name for file for logging, if empty,
   * file is not written
   */
  [[maybe_unused]]
  static void setupLogs(int argc, char **argv,
                        std::string filePathError = "error.log",
                        std::string warningPathError = "")
  {
    loguru::init(argc, argv);
    // loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
    // loguru::add_file("info.log", loguru::Append, loguru::Verbosity_INFO);
    if (filePathError != "")
      loguru::add_file(filePathError.c_str(), loguru::Append,
                       loguru::Verbosity_INFO);
    if (warningPathError != "")
      loguru::add_file(warningPathError.c_str(), loguru::Append,
                       loguru::Verbosity_WARNING);
  }
  [[maybe_unused]]
  static void setupExitFunctions(void (*fn)())
  {
    const int result_register = std::atexit(fn);
    if(result_register)
    {
      LOG_F(ERROR,"Could not register exit function");
    }

    auto lam = [](int i)
    {
      LOG_F(INFO, "Aborting with status: %d",i);
      std::quick_exit(i);
    };
    //^C
    signal(SIGINT, lam);
    // abort()
    signal(SIGABRT, lam);
    // sent by "kill" command
    signal(SIGTERM, lam);
    //^Z
    signal(SIGTSTP, lam);
  }

  class MyNetworkLogger
  {
  public:
    void log([[maybe_unused]] std::string format,[[maybe_unused]] std::string prefix, std::string message)
    {
      std::unique_lock<std::mutex> lock(lock_);

      std::cout << message << std::endl;
      lock.unlock();
    }

  private:
    std::mutex lock_;
  };

  // Note: this function should be thread safe,
  // as logging can be done from any thread at any time.
  [[maybe_unused]]
  static void log_to_network(void *user_data, const loguru::Message &message)
  {

    MyNetworkLogger *logger = reinterpret_cast<MyNetworkLogger *>(user_data);
    logger->log("%s%s", message.prefix, message.message);
  }
  class UploaderErrorLogger
  {
  public:
    UploaderErrorLogger(std::shared_ptr<zmq::socket_t> socket, ZMQConnectInfo &zmqci, std::string serviceName) : socket_(socket), zmqci_(zmqci), serviceName_(serviceName)
    {
    }
    void log(std::string, std::string prefix, std::string message)
    {
      try
      {
      std::unique_lock<std::mutex> lock(lock_);

      json errorMessage = {{"service", serviceName_},
                           {"errorMessage", message},
                           {"errorStack", prefix},
                           {"level", "error"}};
      if (*socket_)
      {
        zmqgain_.sendJSONWithSubject(socket_,
                                     "error", errorMessage, zmqci_);
      }
      lock.unlock();

      }
      catch(const std::exception& e)
      {
        //FIX Right behavior on lock
        LOG_F(3,"%s", e.what() );
      }
      
     
    }

  private:
    std::mutex lock_ = std::mutex();
    std::shared_ptr<zmq::socket_t> socket_;
    ZMQHelper zmqgain_;
    ZMQConnectInfo zmqci_;
    std::string serviceName_;
  };

  // Note: this function should be thread safe,
  // as logging can be done from any thread at any time.
  [[maybe_unused]]

  static void log_to_error(void *user_data, const loguru::Message &message)
  {

    UploaderErrorLogger *logger = reinterpret_cast<UploaderErrorLogger *>(user_data);
    logger->log("%s%s", message.prefix, message.message);
  }

  class Helper
  {

  public:
    std::string storePath;
    /**
     * @brief Calculates the map for the capture ids based on cams and recipe
     *
     * @param cams Definition of cams with positions
     * @param component For correlation for grid (cols,rows)
     * @return std::map<int, std::vector<std::string>>
     */
    std::map<std::string, std::vector<std::string>>
    
    loadMachineConfiguration(std::string filename);
    std::optional<json> readparameters(std::string settingsfile);

    /**
     * @brief
     *
     * @param image OpenCV Image to be stored
     * @param path absolute or relative path
     * @param filename filename (timestamp is added)
     * @param JPEGQuality If set to value between 1 and 100, image is stored as jpeg in this quality
     * @param addTimestamp If true, timestamp is added to filenam
     * @return std::string
     */
    std::string storeImage(cv::Mat image, std::string path, std::string filename,
                           int JPEGQuality = -1, bool addTimestamp = true);
    /**
     * @brief Stores JSON in a file
     *
     * @param filename Filepath for export
     * @param j JSON Input
     * @return true File was written
     * @return false File was not written
     */
    bool storeJSON(const std::string filename, const json &j) const;
    std::optional<json> readJSON(std::string filename);

   

    /**
     * @brief Splits string
     *
     * @param inputString
     * @param delimiter
     * @return std::vector<std::string>
     */
    std::vector<std::string> splitbyCharacter(const std::string &inputString,
                                              char delimiter);
    
    

    unsigned long getUnixTimestamp() const
    {
      std::time_t t = std::time(nullptr);
      return static_cast<unsigned long>(t);
    }
    /**
     * @brief Get the Date String object
     *
     * @param unixTimestamp in seconds
     * @return std::string Format YYYY/mm/dd/HH
     */
    const std::string getDateString(unsigned long int unixTimestamp) const
    {
      std::time_t t = static_cast<std::time_t>(unixTimestamp);

      auto tm = *std::localtime(&t);
      std::ostringstream oss;

      oss << std::put_time(&tm, "%Y/%m/%d/%H");
      return oss.str();
    }
    /**
     * @brief Get the Time Stamp in milliseconds
     *
     * @return long
     */
    long getTimeStampMilliseconds() const
    {
      std::chrono::milliseconds ms =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch());
      return ms.count();
    }
    /**
     * @brief Checks if string is a number
     *
     * @param s
     * @return true
     * @return false
     */

    bool is_number(const std::string &s);

    bool castEnvString(const std::string envName, std::string &value);
    bool castEnvBool(const std::string envName, bool &value, bool defaultTrue = false);
    bool castEnvInteger(const std::string envName, int &value);
    bool castEnvUnsignedInteger(const std::string envName,unsigned int &value);

    bool castEnvSizeT(const std::string envName, size_t &value);

    bool castEnvDouble(const std::string envName, double &value);
    bool castEnvBoolAndString(const std::string envName, std::string &value, bool &bvalue);
    bool castEnvBoolAndInteger(const std::string envName, int &value, bool &bvalue);

    int checkRoot();

    bool compare_files(const std::string &filename1, const std::string &filename2) const;
    void copyFile(const std::string &source, const std::string &dest) const;
   



    /**
     * @brief Check if element exists in vector
     * 
     * @tparam T 
     * @param v 
     * @param key 
     * @return true  
     * @return false 
     */
    template <typename T>
    bool vectorContains(const std::vector<T> &v, const T &key)
    {
      for (size_t i = 0; i < v.size(); i++)
      {
        if (v[i] == key)
          return true;
      }
      return false;
    }
  };
} // namespace opticus
