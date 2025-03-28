#include "wifi_handlers.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "freertos/semphr.h"
#include "esp_rom_gpio.h"

#include "led.h"

static unsigned int retry_num = 0; // conta o numero de tentativas
SemaphoreHandle_t wifi_connected_sem; // isso é extern em main.c

void wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    if(event_base == WIFI_EVENT) {
        switch(event_id) {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "->> Connecting to WiFi...");
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "->> Connected to WiFi.");
                blink_led_success();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(TAG, "->> Disconnected from WiFi.");
                if(retry_num < 5) {
                    esp_wifi_connect();
                    retry_num++;
                    ESP_LOGI(TAG, "->> Retrying connection...");
                } else {
                    ESP_LOGE(TAG, "->> Failed to connect.");
                }
                xSemaphoreTake(wifi_connected_sem, 0); 
                break;
            case IP_EVENT_STA_GOT_IP:
                ESP_LOGI(TAG, "->> Received WiFi ip...");
                blink_led_success();
                break;
            default:
                ESP_LOGW(TAG, "->> Unhandled wifi event: %ld", event_id);
                break;
        }
    } else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "IP received: " IPSTR, IP2STR(&event->ip_info.ip));
        xSemaphoreGive(wifi_connected_sem);
        blink_led_success();
    }
}

void wifi_connect() {
    wifi_connected_sem = xSemaphoreCreateBinary();
    esp_netif_init(); // inicializa a interface de rede
    esp_event_loop_create_default(); // cuida dos eventos
    esp_netif_create_default_wifi_sta(); // configura as estruturas de dados necessárias para a interface de wifi
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    // Registra os handlers de eventos
    esp_event_handler_instance_t instance_wifi;
    esp_event_handler_instance_t instance_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_wifi);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_ip);
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK, // usa criptografia para ser mais seguro
        },
    };
    
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config); // initializa o wifi
    esp_wifi_set_storage(WIFI_STORAGE_RAM); // Opcional, mas recomendado
    esp_wifi_set_ps(WIFI_PS_NONE); // Desativa power-saving para maior estabilidade
    esp_wifi_start();
} 
