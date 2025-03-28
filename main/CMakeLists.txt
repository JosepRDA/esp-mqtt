idf_component_register(
    SRCS "mqtt_handlers.c" "wifi_handlers.c" "main.c" "led.c"
    INCLUDE_DIRS "."
    REQUIRES 
        cJSON
        esp_wifi
        nvs_flash
        esp_event
        driver
        mqtt
)
