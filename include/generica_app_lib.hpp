#pragma once
#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <loguru.hpp>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <netdb.h>
using json = nlohmann::json;

namespace Opticus
{

    enum class ParameterType
    {
        INTEGER,
        STRING,
        FLOAT,
        DOUBLE,
        BOOL,
        COMMAND,
        MENU,
        CATEGORY,
        REGISTER,
        UNKNOWN,
        U8_COMPOUND,
        U16_COMPOUND,
        U32_COMPOUND,
        JSON,
        SPECIAL_COMPOUND
    };

    enum class DisplayLevel
    {
        BEGINNER,
        EXPERT,
        GURU
    };
    class BaseControl
    {
    public:
        std::string name = "";

        std::string description = "";
        ParameterType type = ParameterType::UNKNOWN;
        bool readOnly = false;
        bool disabled = false;
        bool visible = true;
        unsigned int elems = 1;
        DisplayLevel displaylevel = DisplayLevel::BEGINNER;
        bool runtimeParameter = true;
        unsigned int id = 0;
        unsigned int order = 0;

        void *value_ptr = nullptr;
        size_t value_size = 0;
    };

    static void to_json(json &j, const BaseControl &p)
    {
        j = json{{"description", p.description},
                 {"type", p.type},
                 {"name", p.name},
                 {"readOnly", p.readOnly},
                 {"disabled", p.disabled},
                 {"visible", p.visible},
                 {"elems", p.elems},
                 {"displaylevel", p.displaylevel},
                 {"runtimeParameter", p.runtimeParameter},
                 {"id", p.id},
                 {"order", p.order}};
    };
    static void from_json(const json &j, BaseControl &p)
    {
        p.type = j.at("type").get<ParameterType>();
        p.name = j.at("name").get<std::string>();

        if (j.contains("description"))
            p.description = j.at("description").get<std::string>();
        if (j.contains("readOnly"))
            p.readOnly = j.at("readOnly").get<bool>();
        if (j.contains("disabled"))
            p.disabled = j.at("disabled").get<bool>();
        if (j.contains("elems"))
            p.elems = j.at("elems").get<unsigned int>();
        if (j.contains("displaylevel"))
            p.displaylevel = j.at("displaylevel").get<DisplayLevel>();
        if (j.contains("runtimeParameter"))
            p.runtimeParameter = j.at("runtimeParameter").get<bool>();
    };
    template <typename T>
    class Control : public BaseControl
    {
    private:
        T value;

    public:
        Control()
        {
            value_ptr = &value;
            value_size = sizeof(T);
        }
        Control(std::string name, std::string description, T defaultValue, bool readOnly = false, bool disabled = false, unsigned int elems = 1, DisplayLevel displaylevel = DisplayLevel::BEGINNER, bool runtimeParameter = true)
        {
            Control();
            this->name = name;
            this->description = description;
            this->defaultValue = defaultValue;
            this->set_value(defaultValue);
            this->readOnly = readOnly;
            this->disabled = disabled;
            this->elems = elems;
            this->displaylevel = displaylevel;
            this->runtimeParameter = runtimeParameter;
        }

        Control(const Control &other) : BaseControl(other)
        {
            value = other.value;
            defaultValue = other.defaultValue;
            min = other.min;
            max = other.max;
            step = other.step;
            menu = other.menu;
            if (other.connectedValue)
                connectedValue = std::make_unique<T>(*other.connectedValue);
        }

        Control &operator=(const Control &other)
        {
            if (this != &other)
            {
                BaseControl::operator=(other);
                value = other.value;
                defaultValue = other.defaultValue;
                min = other.min;
                max = other.max;
                step = other.step;
                menu = other.menu;
                if (other.connectedValue)
                    connectedValue = std::make_unique<T>(*other.connectedValue);
                else
                    connectedValue.reset();
            }
            return *this;
        }

        T defaultValue;
        T min;
        T max;
        T step;

        std::map<T, std::string> menu;

        std::unique_ptr<T> connectedValue;

        void set_value(T newValue)
        {
            value = newValue;
            value_ptr = &value;
            if (connectedValue)
                *connectedValue = value;
        }
        T get_value() const
        {
            return value;
        }

        size_t size() const
        {
            return sizeof(T);
        }
    };

    [[maybe_unused]] static Control<int> CreateShortControlInt(std::string name, std::string description, int defaultValue, int min = 0, int max = 255, int step = 1, bool readOnly = false, bool disabled = false, unsigned int elems = 1, DisplayLevel displaylevel = DisplayLevel::BEGINNER, bool runtimeParameter = false)
    {
        Control<int> control;
        control.name = name;
        control.description = description;
        control.defaultValue = defaultValue;
        control.set_value(defaultValue);
        control.min = min;
        control.max = max;
        control.step = step;
        control.readOnly = readOnly;
        control.disabled = disabled;
        control.elems = elems;
        control.type = ParameterType::INTEGER;
        control.displaylevel = displaylevel;
        control.runtimeParameter = runtimeParameter;
        return control;
    }
    static Control<bool> CreateBoolControl(std::string name, std::string description, bool defaultValue, bool readOnly = false, bool disabled = false, unsigned int elems = 1, DisplayLevel displaylevel = DisplayLevel::BEGINNER, bool runtimeParameter = true)
    {
        Control<bool> control;
        control.name = name;
        control.description = description;
        control.defaultValue = defaultValue;
        control.set_value(defaultValue);
        control.readOnly = readOnly;
        control.disabled = disabled;
        control.elems = elems;
        control.type = ParameterType::BOOL;
        control.displaylevel = displaylevel;
        control.runtimeParameter = runtimeParameter;
        return control;
    }
    static Control<json> CreateJSONControl(std::string name, std::string description, json defaultValue, bool readOnly = false, bool disabled = false, unsigned int elems = 1, DisplayLevel displaylevel = DisplayLevel::BEGINNER, bool runtimeParameter = true)
    {
        Control<json> control;
        control.name = name;
        control.description = description;
        control.defaultValue = defaultValue;
        control.set_value(defaultValue);
        control.readOnly = readOnly;
        control.disabled = disabled;
        control.elems = elems;
        control.type = ParameterType::JSON;
        control.displaylevel = displaylevel;
        control.runtimeParameter = runtimeParameter;
        return control;
    }
    using ControlInt = Control<int>;
    using ControlFloat = Control<float>;
    using ControlDouble = Control<double>;
    using ControlBool = Control<bool>;
    using ControlString = Control<std::string>;
    using ControlMenu = Control<std::string>;
    using ControlJSON = Control<json>;

    class Controls
    {
    public:
        std::map<std::string, ControlInt> controlInt;
        std::map<std::string, ControlFloat> controlFloat;
        std::map<std::string, ControlDouble> controlDouble;
        std::map<std::string, ControlBool> controlBool;
        std::map<std::string, ControlString> controlString;
        std::map<std::string, ControlMenu> controlMenu;
        std::map<std::string, ControlJSON> controlJSON;

        json convertControlsToJson() const
        {
            json jControls = json::array();

            for (auto &control : controlInt)
            {
                jControls.push_back(control.second);
            }
            for (auto &control : controlFloat)
            {
                jControls.push_back(control.second);
            }
            for (auto &control : controlDouble)
            {
                jControls.push_back(control.second);
            }
            for (auto &control : controlBool)
            {
                jControls.push_back(control.second);
            }
            for (auto &control : controlString)
            {
                jControls.push_back(control.second);
            }
            for (auto &control : controlMenu)
            {
                jControls.push_back(control.second);
            }
            for (auto &control : controlJSON)
            {
                jControls.push_back(control.second);
            }
            return jControls;
        }
        bool existsControl(const std::string &name)
        {
            if (controlInt.find(name) != controlInt.end())
            {
                return true;
            }
            if (controlFloat.find(name) != controlFloat.end())
            {
                return true;
            }
            if (controlDouble.find(name) != controlDouble.end())
            {
                return true;
            }
            if (controlBool.find(name) != controlBool.end())
            {
                return true;
            }
            if (controlString.find(name) != controlString.end())
            {
                return true;
            }
            if (controlMenu.find(name) != controlMenu.end())
            {
                return true;
            }
            if (controlJSON.find(name) != controlJSON.end())
            {
                return true;
            }
            return false;
        }
        size_t totalSize()
        {
            size_t size = 0;
            for (auto &control : controlInt)
            {
                size += control.second.size();
            }
            for (auto &control : controlFloat)
            {
                size += control.second.size();
            }
            for (auto &control : controlDouble)
            {
                size += control.second.size();
            }
            for (auto &control : controlBool)
            {
                size += control.second.size();
            }
            for (auto &control : controlString)
            {
                size += control.second.size();
            }
            for (auto &control : controlMenu)
            {
                size += control.second.size();
            }
            for (auto &control : controlJSON)
            {
                size += control.second.size();
            }
            return size;
        }
        std::vector<std::shared_ptr<BaseControl>> getAllControls()
        {
            std::vector<std::shared_ptr<BaseControl>> controls;
            std::map<int, std::shared_ptr<BaseControl>> orderedControls;

            for (auto &control : controlInt)
            {
                orderedControls[control.second.order] = std::make_shared<BaseControl>(control.second);
            }
            for (auto &control : controlFloat)
            {
                orderedControls[control.second.order] = std::make_shared<BaseControl>(control.second);
            }
            for (auto &control : controlDouble)
            {
                orderedControls[control.second.order] = std::make_shared<BaseControl>(control.second);
            }
            for (auto &control : controlBool)
            {
                orderedControls[control.second.order] = std::make_shared<BaseControl>(control.second);
            }
            for (auto &control : controlString)
            {
                orderedControls[control.second.order] = std::make_shared<BaseControl>(control.second);
            }
            for (auto &control : controlMenu)
            {
                orderedControls[control.second.order] = std::make_shared<BaseControl>(control.second);
            }
            for (auto &control : controlJSON)
            {
                orderedControls[control.second.order] = std::make_shared<BaseControl>(control.second);
            }

            for (const auto &control : orderedControls)
            {
                controls.push_back(control.second);
            }

            return controls;
        }
        bool updateControl(json control, bool useDefault = false)
        {
            json value = useDefault ? control["default"] : control["value"];
            ParameterType type = control["type"];
            switch (type)
            {
            case ParameterType::INTEGER:
                if (controlInt.find(control["name"]) != controlInt.end())
                {
                    controlInt[control["name"]].set_value(value);
                    return true;
                }
                else
                {
                    return false;
                }
            case ParameterType::FLOAT:
                if (controlFloat.find(control["name"]) != controlFloat.end())
                {
                    controlFloat[control["name"]].set_value(value);
                    return true;
                }
                else
                {
                    return false;
                }
            case ParameterType::DOUBLE:
                if (controlDouble.find(control["name"]) != controlDouble.end())
                {
                    controlDouble[control["name"]].set_value(value);
                    return true;
                }
                else
                {
                    return false;
                }
            case ParameterType::BOOL:
                if (controlBool.find(control["name"]) != controlBool.end())
                {
                    controlBool[control["name"]].set_value(value);
                    return true;
                }
                    else
                    {
                        return false;
                    }
                case ParameterType::STRING:
                    if (controlString.find(control["name"]) != controlString.end())
                    {
                        controlString[control["name"]].set_value(value);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                case ParameterType::MENU:
                    if (controlMenu.find(control["name"]) != controlMenu.end())
                    {
                        controlMenu[control["name"]].set_value(value);
                        return true;
                    }
                    else
                    {
                        return false;
                    }

                case ParameterType::JSON:
                    if (controlJSON.find(control["name"]) != controlJSON.end())
                    {
                        controlJSON[control["name"]].set_value(value);
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                return false;
            }
        };

        template <typename T>
        static void to_json(json &j, const Control<T> &p) // TODO
        {
            j = static_cast<BaseControl>(p);
            j["value"] = p.get_value();
            j["default"] = p.defaultValue;
            j["min"] = p.min;
            j["max"] = p.max;
            j["step"] = p.step;
            j["menu"] = p.menu;
        };

        class CompoundControl : public BaseControl
        {
        public:
            CompoundControl()
            {
                value_ptr = nullptr;
                value_size = 0;
                type = ParameterType::SPECIAL_COMPOUND;
            }
            Controls controls;
            std::vector<unsigned char> getValue()
            {
                const size_t datasize = controls.totalSize();
                std::vector<unsigned char> data(datasize, 0);
                LOG_F(INFO, "Total size %zu", datasize);
                auto controlsList = controls.getAllControls();
                auto position = data.data();
                for (size_t i = 0; i < controlsList.size(); i++)
                {
                    auto control = controlsList[i];
                    memcpy(position, control->value_ptr, control->value_size);
                    position += control->value_size;
                }
                return data;
            }
            void updateValues(json &values)
            {
                for (auto &control : values)
                {
                    LOG_F(INFO, "Updating control %s", control.dump().c_str());
                    controls.updateControl(control, false);
                }
            }
        };
        [[maybe_unused]] static void to_json(json &j, const CompoundControl &p)
        {
            j = static_cast<BaseControl>(p);
            j["controls"] = p.controls.convertControlsToJson();
        };
        class GenericApp
        {
        public:
            Controls inputs;
            Controls outputs;

            bool outputs_image = true;

            std::string description;
            std::string name;
            std::string version;
            std::unique_ptr<std::function<void(BaseControl)>> callbackUpdateControl;

            json convertToJSON()
            {
                json jInfos;
                jInfos["description"] = description;
                jInfos["name"] = name;
                jInfos["version"] = version;

                jInfos["inputs"] = inputs.convertControlsToJson();

                jInfos["outputs"] = {{"outputs_image", outputs_image},
                                     {"outputs", outputs.convertControlsToJson()}};

                return jInfos;
            }

            std::function<std::optional<std::vector<uint8_t>>(json)> callbackJSON = [&](json meta)
            {
                // LOG_F(INFO, "%s", meta.dump().c_str());

                if (meta.contains("command"))
                {
                    if (meta["command"] == "infos")
                    {
                        return std::move(json::to_msgpack(convertToJSON()));
                    }
                    else if (meta["command"] == "stop")
                    {
                        LOG_F(INFO, "Stop");
                    }
                    else if (meta["command"] == "set_value")
                    {
                        if (inputs.updateControl(meta["control"]))
                        {
                            LOG_F(INFO, "Updated control %s", meta["control"]["name"].get<std::string>().c_str());
                            if (callbackUpdateControl)
                            {
                                BaseControl control = meta["control"];
                                (*callbackUpdateControl)(control);
                            }
                            return std::move(json::to_msgpack(json{{"status", "ok"}}));
                        }
                        else
                        {
                            LOG_F(INFO, "Error updating control %s", meta["control"]["name"].get<std::string>().c_str());
                            return std::move(json::to_msgpack(json{{"status", "error"}}));
                        }
                    }
                }
                return std::vector<uint8_t>{};
            };
        };
    }