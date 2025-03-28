#ifndef WIFI_HANDLERS
#define WIFI_HANDLERS
#include "general.h"

#include "esp_event.h"
// log config

void wifi_event_handler(void* event_handler_arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data);
void wifi_connect();

#endif
