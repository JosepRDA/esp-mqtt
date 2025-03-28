#ifndef MQTT_HANDLERS
#define MQTT_HANDLERS

#include "esp_event.h"

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
void mqtt_app_start(void);
void send_json_data(void);

#endif