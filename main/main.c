// bibliotecas
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// meu
#include "wifi_handlers.h"
#include "mqtt_handlers.h"

#include "nvs_flash.h"

#include "freertos/semphr.h"

#include "led.h"

extern SemaphoreHandle_t wifi_connected_sem;


void app_main(void) {
    
    led_init();
    // iniciando NVS pois é necessário para wifi na esp
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_connect();

    if (xSemaphoreTake(wifi_connected_sem, pdMS_TO_TICKS(30000))) {
        ESP_LOGI(TAG, "->> WiFi connect! Initializing MQTT...");
        blink_led_success();
        mqtt_app_start();
    } else {
        ESP_LOGE(TAG, "->> reconnect failed after 30 seconds.");
    }
    
    mqtt_app_start();

    ESP_LOGI("INIT", "->> inicializando programa...");
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    while(1) {
        send_json_data();
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
