idf_component_register(
    SRCS "mqtt_handlers.c" "wifi_handlers.c" "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        cJSON
        esp_wifi
        nvs_flash
        esp_event
        #lwip
        mqtt
)
