// bibliotecas
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "lwip/err.h" //light weight ip packets error handling
#include "lwip/sys.h" //system applications for light weight ip apps
#include "cJSON.h"

// log config
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"

static const char* TAG = "WIFI_MQTT_DEMO";

/* Configurações */
#define WIFI_SSID "José"
#define WIFI_PASS "05112007"
#define MQTT_BROKER "mqtt://broker.hivemq.com:1883"
#define PUB_TOPIC "senai/esp32/data"
#define SUB_TOPIC "senai/esp32/comandos"

static void wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data);
static void wifi_connect();

void app_main(void) {

    while(1) {
    
    }
}

static void wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    int retry_num = 0; // conta o numero de tentativas
    switch(event_id) {
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG, "->> Connecting to WiFi...");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "->> Connected to WiFi.");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            ESP_LOGI(TAG, "->> Wifi disconnected.");
            if(retry_num < 5) {
                esp_wifi_connect();
                retry_num++;
                ESP_LOGI(TAG, "->> Retrying connection...");
            }
            break;
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG, "->> Received WiFi ip...");
            break;
    }
}

static void wifi_connect() {
    esp_netif_init(); // inicializa a interface de rede
    esp_event_loop_create_default(); // cuida dos eventos
    esp_netif_create_default_wifi_sta(); // configura as estruturas de dados necessárias para a interface de wifi
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .scan_method = WIFI_FAST_SCAN,
            .bssid_set = 0,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .threshold.rssi = -70,
        },
    };

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config); // initializa o wifi
} 
