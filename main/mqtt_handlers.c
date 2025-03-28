#include "mqtt_handlers.h"

#include "esp_event.h"
#include "general.h"
#include "mqtt_client.h"
#include "cJSON.h"


static esp_mqtt_client_handle_t mqtt_client;

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "->> MQTT connected!");
             
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            esp_mqtt_client_subscribe(mqtt_client, SUB_TOPIC, 0);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "->> Data received: Topic=%.*s, Message=%.*s",
                     event->topic_len, event->topic,
                     event->data_len, event->data);
                     
             
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGE(TAG, "->> MQTT disconnected. reconnecting...");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            esp_mqtt_client_reconnect(mqtt_client); // Força reconexão
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "->> MQTT error: type=%d, code=%d", 
                     event->error_handle->error_type, 
                     event->error_handle->esp_tls_last_esp_err);
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_ESP_TLS) {
                ESP_LOGE(TAG, "Erro TLS: esp_tls_last_esp_err=0x%x", event->error_handle->esp_tls_last_esp_err);
            }
            break;
        default:
            break;
    }
}

void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address.uri = MQTT_BROKER,
            .verification.skip_cert_common_name_check = true, // Ignora validação de CN
        },
        .network = {
            .reconnect_timeout_ms = 10000,        // Timeout de reconexão
            .disable_auto_reconnect = false,      // Habilita autoreconexão
        },
        .session = {
            .keepalive = 60,                      // Keepalive aumentado
            .disable_clean_session = false,       // Mantém sessão
        },
        .buffer = {
            .size = 2048,                         // Buffer maior
            .out_size = 1024,                     // Buffer de saída
        },
        .task = {
            .priority = configMAX_PRIORITIES - 2, // Prioridade alta
            .stack_size = 6144,                   // Stack aumentada
        }
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

void send_json_data(void) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "device_id", "ESP32_SENAI");
    cJSON_AddNumberToObject(root, "temperature", 25.5);
    cJSON_AddNumberToObject(root, "humidity", 60.2);
    cJSON_AddStringToObject(root, "status", "active");

    char *json_str = cJSON_PrintUnformatted(root);
    esp_mqtt_client_publish(mqtt_client, PUB_TOPIC, json_str, 0, 0, 0);
    ESP_LOGI(TAG, "->> Sent JSON: %s", json_str);

    cJSON_Delete(root);
    free(json_str);
}