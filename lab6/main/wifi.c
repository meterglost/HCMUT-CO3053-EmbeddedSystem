/**
 * Connect to wifi as instructions from ESP-IDF
 * - [Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html#esp32-wi-fi-station-general-scenario)
 * - [Example](https://github.com/espressif/esp-idf/blob/master/examples/wifi/getting_started/station/main/station_example_main.c)
 */

#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"

#include "esp_mac.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "wifi.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

const char * LOG_TAG = "wifi station";

void init_wifi_config_storage(void)
{
    ESP_ERROR_CHECK(nvs_flash_erase());
    ESP_ERROR_CHECK(nvs_flash_init());
}

#if defined(WIFI_ST_SSID) && defined(WIFI_ST_PASS)

void wifi_st_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    static int s_retry_times = 5;

    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        if (s_retry_times > 0) {
            --s_retry_times;
            esp_wifi_connect();
            ESP_LOGI(LOG_TAG, "Retry to connect to the Wifi...");
        } else {
            ESP_LOGI(LOG_TAG, "Failed to connect to the Wifi.");
        }
        break;

    case WIFI_EVENT_STA_CONNECTED:
        s_retry_times = 5;
        break;

    case IP_EVENT_STA_GOT_IP:
        s_retry_times = 5;
        break;

    default:
        break;
    }
}

void init_wifi_st()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_st_event_handler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_st_event_handler, NULL, &instance_got_ip));

    wifi_config_t wifi_config = { {}, .sta = {
        .ssid = WIFI_ST_SSID,
        .password = WIFI_ST_PASS,
    }, };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
}

void connect_wifi()
{
    init_wifi_config_storage();
    init_wifi_st();
    ESP_ERROR_CHECK(esp_wifi_start());
}

#endif

#if defined(WIFI_AP_SSID) && defined(WIFI_AP_PASS)

void wifi_ap_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_AP_STACONNECTED:
        wifi_event_ap_staconnected_t* event_connect = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(LOG_TAG, "station "MACSTR" join, AID=%d", MAC2STR(event_connect->mac), event_connect->aid);
        break;

    case WIFI_EVENT_AP_STADISCONNECTED:
        wifi_event_ap_stadisconnected_t* event_disconnect = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(LOG_TAG, "station "MACSTR" leave, AID=%d", MAC2STR(event_disconnect->mac), event_disconnect->aid);
        break;
    }
}

void init_wifi_ap()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_ap_event_handler, NULL, NULL));

    wifi_config_t wifi_config = { {}, .ap = {
        .ssid = WIFI_AP_SSID,
        .password = WIFI_AP_PASS,
        .authmode = WIFI_AUTH_WPA3_PSK,
        .max_connection = 1,
    }, };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
}

void create_wifi()
{
    init_wifi_config_storage();
    init_wifi_ap();
    ESP_ERROR_CHECK(esp_wifi_start());
}

#endif
