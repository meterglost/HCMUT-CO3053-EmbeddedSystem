/**
 * Connect to wifi as instructions from ESP-IDF
 * - [Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-station-general-scenario)
 * - [Example](https://github.com/espressif/esp-idf/blob/master/examples/wifi/getting_started/station/main/station_example_main.c)
 */

#ifndef __WIFI_H__
#define __WIFI_H__

// #define WIFI_ST_SSID ""
// #define WIFI_ST_PASS ""

#define WIFI_AP_SSID "ESP32-AP-TEST"
#define WIFI_AP_PASS "12345678"

#if defined(WIFI_ST_SSID) && defined(WIFI_ST_PASS)
void connect_wifi();
#endif

#if defined(WIFI_AP_SSID) && defined(WIFI_AP_PASS)
void create_wifi();
#endif

#endif
