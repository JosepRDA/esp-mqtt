idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        cJSON
        esp_wifi
        nvs_flash
        esp_event
        lwip
        mqtt
)
