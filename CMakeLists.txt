idf_component_register(
    INCLUDE_DIRS
    "system_tools/include" 
    "pir/include"
    "ota_tools/include"
    "OpenWeather/include"
    "nvs_tools/include"
    "mqtt_class/include"
    "hass/include"
    "hass/virtual_switch/include"
    "encoder/include"
    "encoder/priv_include/"
    "boot_tools/include" "blind/include"
    "ambient_sensor/include"
    "lnohmann_json/"
    "lnohmann_json/nlohmann"
    
    SRC_DIRS    
    "system_tools/src"
    "pir/src"
    "ota_tools/src"
    "OpenWeather/src"
    "nvs_tools/src"
    "mqtt_class/src"    "hass/src"
    "hass/virtual_switch/src" 
    "encoder/src" 
    "encoder/priv_include/" 
    "boot_tools/src"
    "blind/src"
    "ambient_sensor/src"


    PRIV_INCLUDE_DIRS "mqtt_class/include/priv_include" 
    REQUIRES arduino-esp32 
    PRIV_REQUIRES  bootloader_support app_update libsodium pthread mqtt
                     )