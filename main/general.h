#ifndef GENERAL
#define GENERAL

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

/* Configurações */
#define TAG "WIFI_MQTT_DEMO"
#define WIFI_SSID "locolul"
#define WIFI_PASS "clek8982"
#define MQTT_BROKER "mqtt://test.mosquitto.org"
#define PUB_TOPIC "senai/esp32/data"
#define SUB_TOPIC "senai/esp32/comandos"

#endif