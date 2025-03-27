#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "cJSON.h"

#define TAG "MQTT_DEMO"

/* Configurações */
#define WIFI_SSID "SUA_REDE_WIFI"
#define WIFI_PASS "SUA_SENHA"
#define MQTT_BROKER "mqtt://broker.hivemq.com"
#define PUB_TOPIC "seu-nome/esp32/data"
#define SUB_TOPIC "seu-nome/esp32/comandos"

static esp_mqtt_client_handle_t client;

/* WiFi Event Handler */
static void wifi_event_handler(void* arg, esp_event_base_t event_base, 
                             int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Tentando reconectar ao WiFi...");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Conectado com IP: " IPSTR, IP2STR(&event->ip_info.ip));
    }
}

/* Inicializa WiFi */
void wifi_init() {
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

/* MQTT Event Handler */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, 
                              int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado ao Broker MQTT");
            esp_mqtt_client_subscribe(client, SUB_TOPIC, 0);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Dados recebidos: %.*s", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "Erro MQTT");
            break;
        default:
            break;
    }
}

/* Publica dados simulados */
void publish_mock_data() {
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_AddArrayToObject(root, "dados");
    
    cJSON_AddItemToArray(array, cJSON_CreateString("Sensor1: OK"));
    cJSON_AddItemToArray(array, cJSON_CreateString("Temperatura: 23.5C"));
    cJSON_AddItemToArray(array, cJSON_CreateString("Status: Ativo"));
    
    char *payload = cJSON_PrintUnformatted(root);
    esp_mqtt_client_publish(client, PUB_TOPIC, payload, 0, 0, 0);
    ESP_LOGI(TAG, "Dados publicados: %s", payload);
    
    cJSON_Delete(root);
    free(payload);
}

void app_main() {
    // Inicializa NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Inicializa WiFi
    wifi_init();

    // Configura cliente MQTT
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_BROKER,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

    // Loop principal
    while (1) {
        publish_mock_data();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
