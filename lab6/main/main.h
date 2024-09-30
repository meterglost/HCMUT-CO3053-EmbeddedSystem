#pragma once

// #define WIFI_ST_SSID ""
// #define WIFI_ST_PASS ""

#define WIFI_AP_SSID "ESP32-AP"
#define WIFI_AP_PASS "123456#@"

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

void init_storage(void);

void init_wifi(void);
