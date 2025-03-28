
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_GPIO 2  // Pino do LED (ajuste conforme sua placa)

// Implementações
void led_init() {
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);  // Inicia desligado
}

void blink_led_success() {
    for (int i = 0; i < 2; i++) {
        gpio_set_level(LED_GPIO, 1);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        gpio_set_level(LED_GPIO, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}