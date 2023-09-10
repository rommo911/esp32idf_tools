
#pragma once 
#ifndef _HOMEASSISTANT_
#define _HOMEASSISTANT_


#include <forward_list>
#include <memory>
#include "nlohmann/json.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <esp_log.h>
#include <functional>
#include "esp_err.h"

#ifndef HOMEASSISTANT_PREFIX
#define HOMEASSISTANT_PREFIX    "homeassistant" // Default MQTT prefix
#endif

#ifndef HOMEASSISTANT_RETAIN
#define HOMEASSISTANT_RETAIN    true     // Make broker retain the messages
#endif


namespace homeassistant {
    class Switch_Class_t
    {
    public:
        static constexpr char	None[] = "None";
        static constexpr char	outlet[] = "outlet";
        static constexpr char	Switch[] = "switch";
    };

    typedef struct {
        std::string name;
        std::string version;
        std::string manufacturer;
        std::string room;
        std::string MAC;
        std::string model;
        std::string config_url;
        std::string suggested_area;
        std::string connections;
    } Device_Description_t;

    class BaseDevCtx {
    protected:
        Device_Description_t &deviceDescription;
        nlohmann::json _json{};
    public:
        BaseDevCtx(Device_Description_t &des);
        void SetDescription(Device_Description_t &des);
        void UpdateDescription();
        const std::string& name()const;
        const std::string& room()const;
        const std::string& MAC()const;
        nlohmann::json JsonObject()const;
    };

    class Discovery {
    protected:
        BaseDevCtx& _BaseDevCtx;
        std::string hass_mqtt_device;
        std::stringstream discovery_topic;
        std::string availability_topic = "";
        std::string state_topic = "";
        std::string command_topic = "";
        std::string discovery_message;
        std::stringstream topics_prefix;
        std::stringstream unique_id;
        nlohmann::json discoveryJson;
        virtual void ProcessFinalJson() = 0;
        bool procced = false;
    protected:
        static constexpr char relay_t[] = "switch";
        static constexpr char cover_t[] = "cover";
        static constexpr char light_t[] = "light";
        static constexpr char binary_sensor_t[] = "binary_sensor";
        static constexpr char sensor_t[] = "sensor";
    public:

        Discovery(const std::string& _hass_mqtt_device);
        virtual ~Discovery() {
        }
        void ProcessJson();
        void UpdateDevCtx();
        const std::string DiscoveryTopic() const;
        const std::string& AvailabilityTopic() const;
        const std::string StatusTopic() const;
        const std::string CommandTopic() const;
        const std::string& DiscoveryMessage() const;
        const  std::string& ConnectionTopic() const;
        const std::pair<std::string,std::string> AvailabilityMessage()const ;
        const std::pair<std::string, std::string> GenerateDiscoveryData() const 
        {
            return std::make_pair(this->DiscoveryTopic(), this->DiscoveryMessage());
        }

        void DumpDebugAll();
    };

    esp_err_t UpdateDiscoveryList();
    esp_err_t PublishDiscovery_and_Available();
    esp_err_t PublishAllStats();
    extern std::vector<std::function<const std::pair<std::string,std::string>()>> statsGenerateFunctions;
    extern std::function<esp_err_t(const std::pair<std::string,std::string>)> MqttPublishFunction;
    extern std::function<esp_err_t(const std::pair<std::string,std::string>)> statsPublishDiscoveryFunction;
    extern homeassistant::Device_Description_t hassDevCtxDescription;
    extern homeassistant::BaseDevCtx thisDevideCtx;


    
    class RelayDiscovery : public Discovery {
    public:
        RelayDiscovery(const std::string& switch_name, const std::string& class_type) : Discovery(relay_t), _switch_name(switch_name), _class_type(class_type)
        {
        }
        static constexpr char Switch[] = "Switch";
        const std::string& GetName() const { return _switch_name; }
    private:
        void ProcessFinalJson();
        const std::string _switch_name;
        const std::string _class_type;
    };



    class BlindDiscovery : public Discovery {
    private:
        std::string setPosTopic, positionTopic, _class_type;
        void ProcessFinalJson();

    public:
        BlindDiscovery(const char* class_type) : Discovery(Discovery::cover_t), _class_type(class_type)
        {
        }
        const std::string GetSetPosTopic() const { return  std::string(topics_prefix.str() + "/set_pos"); }
        const std::string GetPosTopic() const { return  std::string(topics_prefix.str() + "/position"); }
        static constexpr char	None[] = "None";
        static constexpr char	awning[] = "awning";
        static constexpr char	blind[] = "blind";
        static constexpr char	curtain[] = "curtain";
        static constexpr char	damper[] = "damper";
        static constexpr char	door[] = "door";
        static constexpr char	gate[] = "gate";
        static constexpr char	shade[] = "shade";
        static constexpr char	shutter[] = "shutter";
        static constexpr char	window[] = "window";
    };

    class SensorDiscovery : public Discovery {
    private:
        std::string name;
        std::string _sensorClass;
        std::string __unit;
        void ProcessFinalJson();

    public:
        SensorDiscovery(const char* sensorClass, const std::string& unit) : Discovery(sensor_t), name(sensorClass), _sensorClass(sensorClass), __unit(unit)
        {
        }
        const std::string& GetClass() const { return name; }
        static constexpr char	aqi[] = "aqi";
        static constexpr char	battery[] = "battery";
        static constexpr char	carbon_dioxide[] = "carbon_dioxide";
        static constexpr char	carbon_monoxide[] = "carbon_monoxide";
        static constexpr char	current[] = "current";
        static constexpr char	energy[] = "energy";
        static constexpr char	frequency[] = "frequency";
        static constexpr char	gas[] = "gas";
        static constexpr char	humidity[] = "humidity";
        static constexpr char	illuminance[] = "illuminance";
        static constexpr char	monetary[] = "monetary";
        static constexpr char	nitrogen_dioxide[] = "nitrogen_dioxide";
        static constexpr char	nitrogen_monoxide[] = "nitrogen_monoxide";
        static constexpr char	nitrous_oxide[] = "nitrous_oxide";
        static constexpr char	ozone[] = "ozone";
        static constexpr char	pm1[] = "pm1";
        static constexpr char	pm10[] = "pm10";
        static constexpr char	pm25[] = "pm25";
        static constexpr char	power_factor[] = "power_factor";
        static constexpr char	power[] = "power";
        static constexpr char	pressure[] = "pressure";
        static constexpr char	signal_strength[] = "signal_strength";
        static constexpr char	sulphur_dioxide[] = "sulphur_dioxide";
        static constexpr char	temperature[] = "temperature";
        static constexpr char	timestamp[] = "timestamp";
        static constexpr char	volatile_organic_compounds[] = "volatile_organic_compounds";
        static constexpr char	voltage[] = "voltage";
        static constexpr char	None[] = "None";

    };
    class BinarySensorDiscovery : public Discovery {
    private:
        std::string name;
        std::string _sensorClass;
        void ProcessFinalJson();

    public:
        BinarySensorDiscovery(const char* sensorClass) : Discovery(binary_sensor_t), name(sensorClass), _sensorClass(sensorClass)
        {
        }
        const std::string& GetClass() const { return name; }
        static constexpr char	None[] = "None";
        static constexpr char	battery[] = "battery";
        static constexpr char	battery_charging[] = "battery_charging";
        static constexpr char	cold[] = "cold";
        static constexpr char	connectivity[] = "connectivity";
        static constexpr char	door[] = "door";
        static constexpr char	garage_door[] = "garage_door";
        static constexpr char	gas[] = "gas";
        static constexpr char	heat[] = "heat";
        static constexpr char	light[] = "light";
        static constexpr char	lock[] = "lock";
        static constexpr char	moisture[] = "moisture";
        static constexpr char	motion[] = "motion";
        static constexpr char	moving[] = "moving";
        static constexpr char	occupancy[] = "occupancy";
        static constexpr char	opening[] = "opening";
        static constexpr char	plug[] = "plug";
        static constexpr char	power[] = "power";
        static constexpr char	presence[] = "presence";
        static constexpr char	problem[] = "problem";
        static constexpr char	running[] = "running";
        static constexpr char	safety[] = "safety";
        static constexpr char	smoke[] = "smoke";
        static constexpr char	sound[] = "sound";
        static constexpr char	tamper[] = "tamper";
        static constexpr char	update[] = "update";
        static constexpr char	vibration[] = "vibration";
        static constexpr char	window[] = "window";

    };
};

#endif