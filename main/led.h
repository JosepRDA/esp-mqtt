#ifndef LED
#define LED

#include "driver/gpio.h"

// Declarações públicas
void led_init(void);              // Inicializa o GPIO do LED
void blink_led_success(void);     // Pisca o LED 2x

#endif