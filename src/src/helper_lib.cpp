#include <algorithm>
#include <filesystem>
#include <fstream>
#include <helper_lib.hpp>
#include <iomanip>
#include <loguru.hpp>
#include <map>
#include <memory>
#include <opencv2/opencv.hpp>
#include <tuple>
namespace fs = std::filesystem;

namespace opticus
{

std::optional<json> Helper::readparameters(std::string settingsfile)
{
    fs::path filepath(settingsfile);
    if (!fs::exists(filepath))
    {
        LOG_F(ERROR, "File \"%s\" does not exist", settingsfile.c_str());
        return std::nullopt;
    }

    std::ifstream i(settingsfile, std::ifstream::in);

    if (i.good())
    {
        json j;
        i >> j;
        std::string s = j.dump();
        LOG_F(3, "%s", s.c_str());
        return j;
    }
    else
    {
        LOG_F(ERROR, "Settingsfile does not exist %s", settingsfile.c_str());
        return std::nullopt;
    }
}
bool Helper::storeJSON(const std::string filename, const json &j) const
{
    try
    {
        std::ofstream out(filename);
        out << j.dump(4);
        out.close();
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "Cannot store file %s", filename.c_str());
        return false;
    }
    return true;
}
std::optional<json> Helper::readJSON(std::string filename)
{
    std::ifstream i(filename, std::ifstream::in);

    if (i.good())
    {
        json j;
        i >> j;
        std::string s = j.dump();
        LOG_F(INFO, "%s", s.c_str());
        return j;
    }
    else
    {
        LOG_F(ERROR, "File %s does not exist;", filename.c_str());
        return nullptr;
    }
}


std::string Helper::storeImage(cv::Mat image,
                               std::string path,
                               std::string filename,
                               int JPEGQuality,
                               bool addTimestamp)
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
    auto datestring = oss.str();
    std::vector<int> params = {};
    // std::vector<int> params = {cv::ImwriteFlags::IMWRITE_PNG_COMPRESSION, 9};
    std::string fileending = ".png";
    if (JPEGQuality >= 0)
    {
        params = {cv::ImwriteFlags::IMWRITE_JPEG_QUALITY, JPEGQuality};
        fileending = ".jpg";
    }

    std::filesystem::path dir(path);
    // std::filesystem::path file();
    // std::filesystem::path full_path = dir / file;
    if (!addTimestamp)
        datestring = "";
    fs::path imagePath = dir / (filename + datestring + fileending);

    std::string fullFilename = imagePath.string();
    if (!cv::imwrite(fullFilename, image, params))
    {
        LOG_F(ERROR, "Image could not be saved: %s", fullFilename.c_str());
        return "";
    }
    else
    {
        // system(("chown elunic "+fullFilename).c_str());
        system(("chmod 777 " + fullFilename).c_str());

        // LOG_F(WARNING, ("Image was saved:" +  fullFilename).c_str());
        return fullFilename;
    }
}
std::vector<std::string> Helper::splitbyCharacter(const std::string &inputString, char delimiter)
{
    std::stringstream ss(inputString);
    std::vector<std::string> result;

    while (ss.good())
    {
        std::string substr;
        std::getline(ss, substr, delimiter);
        result.push_back(substr);
    }
    return result;
}


bool Helper::is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

int Helper::checkRoot()
{

    // auto user = getuid();
    // if (user)
    //     return false;

    // return true;

    return false;
}

bool Helper::compare_files(const std::string &filename1, const std::string &filename2) const
{
    std::ifstream file1(filename1, std::ifstream::ate | std::ifstream::binary); //open file at the end
    std::ifstream file2(filename2, std::ifstream::ate | std::ifstream::binary); //open file at the end
    const std::ifstream::pos_type fileSize = file1.tellg();

    if (fileSize != file2.tellg())
    {
        return false; //different file size
    }

    file1.seekg(0); //rewind
    file2.seekg(0); //rewind

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    return std::equal(begin1, std::istreambuf_iterator<char>(), begin2); //Second argument is end-of-range iterator
}


void Helper::copyFile(const std::string &source, const std::string &dest) const
{
    try
    {
        std::ifstream src(source, std::ios::binary);
        std::ofstream dst(dest, std::ios::binary);

        dst << src.rdbuf();
    }
    catch (const std::exception &e)
    {
        LOG_F(ERROR, "%s", e.what());
    }
}

bool Helper::castEnvString(const std::string envName, std::string &value)
{
    const char *envv = std::getenv(envName.c_str());
    if (envv && strlen(envv) > 0)
    {
        value = envv;
        LOG_F(INFO, "Variable %s: %s", envName.c_str(), value.c_str());

        return true;
    }
    else
    {
        LOG_F(INFO, "Variable %s not set, using default: %s", envName.c_str(), value.c_str());
        return false;
    }
}
bool Helper::castEnvBool(const std::string envName, bool &value, bool defaultTrue)
{
    const char *envv = std::getenv(envName.c_str());
    if (((envv && strlen(envv) > 0) || defaultTrue) &&
        !(envv && (std::strcmp(envv, "OFF") == 0 || std::strcmp(envv, "FALSE") == 0 ||
                   std::strcmp(envv, "false") == 0 || std::strcmp(envv, "off") == 0)))
    {
        value = true;
        LOG_F(INFO, "Variable %s: %s", envName.c_str(), value ? "TRUE" : "FALSE");
        return true;
    }
    else
    {
        value = false;
        LOG_F(INFO, "Variable %s: %s", envName.c_str(), value ? "TRUE" : "FALSE");

        return false;
    }
}
bool Helper::castEnvInteger(const std::string envName, int &value)
{
    const char *envv = std::getenv(envName.c_str());
    if (envv && strlen(envv) > 0)
    {
        try
        {
            value = std::stoi(envv);
            LOG_F(INFO, "Variable %s: %d", envName.c_str(), value);
            return true;
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "For env variable %s: %s", envName.c_str(), e.what());
            return false;
        }
    }
    else
    {
        LOG_F(INFO, "Variable %s not set, using default: %d", envName.c_str(), value);
        return false;
    }
}
bool Helper::castEnvUnsignedInteger(const std::string envName, unsigned int &value)
{
    const char *envv = std::getenv(envName.c_str());
    if (envv && strlen(envv) > 0)
    {
        try
        {
            value = std::stoul(envv);
            LOG_F(INFO, "Variable %s: %d", envName.c_str(), value);
            return true;
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "For env variable %s: %s", envName.c_str(), e.what());
            return false;
        }
    }
    else
    {
        LOG_F(INFO, "Variable %s not set, using default: %d", envName.c_str(), value);
        return false;
    }
}
bool Helper::castEnvSizeT(const std::string envName, size_t &value)
{
    const char *envv = std::getenv(envName.c_str());
    if (envv && strlen(envv) > 0)
    {
        try
        {
            value = std::stoul(envv);
            LOG_F(INFO, "Variable %s: %zu", envName.c_str(), value);

            return true;
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "For env variable %s: %s", envName.c_str(), e.what());
            return false;
        }
    }
    else
    {
        LOG_F(INFO, "Variable %s not set, using default: %zu", envName.c_str(), value);
        return false;
    }
}
bool Helper::castEnvDouble(const std::string envName, double &value)
{
    const char *envv = std::getenv(envName.c_str());
    if (envv && strlen(envv) > 0)
    {
        try
        {
            value = std::stod(envv);
            LOG_F(INFO, "Variable %s: %f", envName.c_str(), value);

            return true;
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "For env variable %s: %s", envName.c_str(), e.what());
            return false;
        }
    }
    else
    {
        LOG_F(INFO, "Variable %s not set, using default: %f", envName.c_str(), value);
        return false;
    }
}
bool Helper::castEnvBoolAndString(const std::string envName, std::string &value, bool &bvalue)
{
    const char *envv = std::getenv(envName.c_str());
    if (envv && strlen(envv) > 0 && !(std::strcmp(envv, "OFF") == 0))
    {
        value = envv;
        bvalue = true;
        LOG_F(INFO, "Variable %s: %s", envName.c_str(), value.c_str());

        return true;
    }
    else
    {
        bvalue = false;
        LOG_F(INFO, "Variable %s not set, using default: %s", envName.c_str(), value.c_str());

        return false;
    }
}
bool Helper::castEnvBoolAndInteger(const std::string envName, int &value, bool &bvalue)
{
    const char *envv = std::getenv(envName.c_str());
    if (envv && strlen(envv) > 0 && !(std::strcmp(envv, "OFF") == 0))
    {
        try
        {
            value = std::stoi(envv);
            bvalue = true;
            LOG_F(INFO, "Variable %s: %d", envName.c_str(), value);

            return true;
        }
        catch (const std::exception &e)
        {
            LOG_F(ERROR, "For env variable %s: %s", envName.c_str(), e.what());
            return false;
        }
    }
    else
    {
        LOG_F(INFO, "Variable %s not set", envName.c_str());
        bvalue = false;
        return false;
    }
}


} // namespace opticus