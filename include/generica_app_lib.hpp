#include <nlohmann/json.hpp>
#include <vector>
#include <memory>
#include <optional>
#include <functional>
#include <loguru.hpp>
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
        JSON
    };

    enum class DisplayLevel
    {
        BEGINNER,
        EXPERT,
        GURU
    };
    struct BaseControl
    {
        std::string name = "";

        std::string description = "";
        ParameterType type = ParameterType::UNKNOWN;
        bool readOnly = false;
        bool disabled = false;
        unsigned int elems = 1;
        DisplayLevel displaylevel = DisplayLevel::BEGINNER;
        bool runtimeParameter = true;
    };

    static void to_json(json &j, const BaseControl &p)
    {
        j = json{{"description", p.description},
                 {"type", p.type},
                 {"name", p.name},
                 {"readOnly", p.readOnly},
                 {"disabled", p.disabled},
                 {"elems", p.elems},
                 {"displaylevel", p.displaylevel},
                 {"runtimeParameter", p.runtimeParameter}};
    };
        static void from_json(const json &j,  BaseControl &p)
    {
        p.type = j.at("type").get<ParameterType>();
        p.name = j.at("name").get<std::string>();
        
        if(j.contains("description"))p.description = j.at("description").get<std::string>();
        if(j.contains("readOnly")) p.readOnly = j.at("readOnly").get<bool>();
        if(j.contains("disabled"))p.disabled = j.at("disabled").get<bool>();
        if(j.contains("elems"))p.elems = j.at("elems").get<unsigned int>();
        if(j.contains("displaylevel"))p.displaylevel = j.at("displaylevel").get<DisplayLevel>();
        if(j.contains("runtimeParameter"))p.runtimeParameter = j.at("runtimeParameter").get<bool>();
    };
    template <typename T>
    struct Control : public BaseControl
    {
        struct MenuEntry
        {
            T value;
            std::string name;
        };

        T value;
        T defaultValue;
        T min;
        T max;
        T step;

        std::vector<MenuEntry> menu;

        std::unique_ptr<T> connectedValue;


        void set_value(T newValue)
        {
            value = newValue;
            if(connectedValue)
            *connectedValue = value;
        }


    };
        
    struct ControlInt : public Control<int>
    {
        ParameterType type = ParameterType::INTEGER;
    };

    using ControlFloat = Control<float>;
    using ControlDouble = Control<double>;
    using ControlBool = Control<bool>;
    using ControlString = Control<std::string>;
    using ControlMenu = Control<std::string>;
    using ControlJSON = Control<json>;

    struct Controls
    {
        std::vector<ControlInt> controlInt;
        std::vector<ControlFloat> controlFloat;
        std::vector<ControlDouble> controlDouble;
        std::vector<ControlBool> controlBool;
        std::vector<ControlString> controlString;
        std::vector<ControlMenu> controlMenu;
        std::vector<ControlJSON> controlJSON;

    };
    template <typename T>
    static void to_json(json &j, const Control<T> &p) //TODO
    {
        j = json{{"description", p.description},
                 {"type", p.type},
                 {"name", p.name},
                 {"readOnly", p.readOnly},
                 {"disabled", p.disabled},
                 {"elems", p.elems},
                 {"displaylevel", p.displaylevel}};
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

        json convertControlsToJson(const Controls &controls)
        {
            json jControls = json::array();
            json jInputInt = controls.controlInt;
            json jInputFloat = controls.controlFloat;
            json jInputDouble = controls.controlDouble;
            json jInputBool = controls.controlBool;
            json jInputString = controls.controlString;
            json jInputMenu = controls.controlMenu;
            json jInputJSON = controls.controlJSON;

            jControls.insert(jControls.begin(), jInputInt.begin(), jInputInt.end());
            jControls.insert(jControls.begin(), jInputFloat.begin(), jInputFloat.end());
            jControls.insert(jControls.begin(), jInputDouble.begin(), jInputDouble.end());
            jControls.insert(jControls.begin(), jInputBool.begin(), jInputBool.end());
            jControls.insert(jControls.begin(), jInputString.begin(), jInputString.end());
            jControls.insert(jControls.begin(), jInputMenu.begin(), jInputMenu.end());
            jControls.insert(jControls.begin(), jInputJSON.begin(), jInputJSON.end());


            return jControls;
        }

        bool updateControl(json control)
        {
            ParameterType type = control["type"];
            switch (type)
            {
            case ParameterType::INTEGER:
                for (auto &c : inputs.controlInt)
                {
                    if (c.name == control["name"])
                    {
                        c.value = control["value"];
                        return true;
                    }
                }
                break;
            case ParameterType::FLOAT:
                for (auto &c : inputs.controlFloat)
                {
                    if (c.name == control["name"])
                    {
                        c.value = control["value"];
                        return true;
                    }
                }
                break;
            case ParameterType::DOUBLE:
                for (auto &c : inputs.controlDouble)
                {
                    if (c.name == control["name"])
                    {
                        c.value = control["value"];
                        return true;
                    }
                }
                break;
            case ParameterType::BOOL:
                for (auto &c : inputs.controlBool)
                {
                    if (c.name == control["name"])
                    {
                        c.value = control["value"];
                        return true;
                    }
                }
                break; 
            case ParameterType::STRING: 
                for (auto &c : inputs.controlString)
                {
                    if (c.name == control["name"])
                    {
                        c.value = control["value"];
                        return true;
                    }
                }
                break;
            case ParameterType::MENU:
                for (auto &c : inputs.controlMenu)
                {
                    if (c.name == control["name"])
                    {
                        c.value = control["value"];
                        return true;
                    }
                }
                break;
            
            case ParameterType::JSON:
                for (auto &c : inputs.controlJSON)
                {
                    if (c.name == control["name"])
                    {
                        c.value = control["value"];
                        return true;
                    }
                }
                break;
            }
            return false;
        }

        json convertToJSON()
        {
            json jInfos;
            jInfos["description"] = description;
            jInfos["name"] = name;
            jInfos["version"] = version;

            jInfos["inputs"] = convertControlsToJson(inputs);

            jInfos["outputs"] = {{"outputs_image", outputs_image},
                                 {"outputs", convertControlsToJson(outputs)}};

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
                else if(meta["command"] == "set_value")
                {
                    if(updateControl(meta["control"]))
                    {
                        LOG_F(INFO, "Updated control %s", meta["control"]["name"].get<std::string>().c_str());
                        if(callbackUpdateControl)
                        {
                            BaseControl control = meta["control"];
                            (*callbackUpdateControl)(control);
                        }
                        return std::move(json::to_msgpack(json{{"status", "ok"}}));

                    }
                    else
                    {
                        return std::move(json::to_msgpack(json{{"status", "error"}}));
                    }
                }
            }
            return std::vector<uint8_t>{};
        };
    };

}