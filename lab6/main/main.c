#include "sdkconfig.h"

#define BOOTLOADER_BUILD 1 // Workaround for bullshit "feature" https://github.com/espressif/esp-idf/issues/8570#issuecomment-1477333244
#define LOG_LOCAL_LEVEL  ESP_LOG_INFO
#include "esp_log.h"

#include "esp_event.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "main.h"

#if defined(WIFI_ST_SSID) && defined(WIFI_ST_PASS)
	#define WIFI_ST_IS_CONFIGURED 1
#endif

#if defined(WIFI_AP_SSID) && defined(WIFI_AP_PASS)
	#define WIFI_AP_IS_CONFIGURED 1
#endif

#if !(defined(WIFI_ST_IS_CONFIGURED)) && !(defined(WIFI_AP_IS_CONFIGURED))
	#error "Wifi is unconfigured"
#endif

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
	switch (event_id)
	{
	case WIFI_EVENT_STA_START:
	{
		wifi_config_t wifi_config;
		esp_wifi_get_config(WIFI_IF_STA, &wifi_config);
		ESP_LOGI("WIFI_ST", "Started: Connect to SSID=%s", wifi_config.sta.ssid);
		esp_wifi_connect();
		break;
	}

	case WIFI_EVENT_STA_CONNECTED:
	{
		ESP_LOGI("WIFI_ST", "Connected");
		break;
	}

	case WIFI_EVENT_STA_DISCONNECTED:
	{
		ESP_LOGW("WIFI_ST", "Disconnected");
		break;
	}

	case WIFI_EVENT_STA_STOP:
	{
		ESP_LOGI("WIFI_ST", "Stopped");
		break;
	}

	case WIFI_EVENT_AP_START:
	{
		wifi_config_t wifi_config;
		esp_wifi_get_config(WIFI_IF_AP, &wifi_config);
		ESP_LOGI("WIFI_AP", "Started: Connect at SSID=%s", wifi_config.ap.ssid);
		break;
	}

	case WIFI_EVENT_AP_STACONNECTED:
	{
		wifi_event_ap_staconnected_t *connection = (wifi_event_ap_staconnected_t *)event_data;
		ESP_LOGI("WIFI_AP", "new connection: MAC=" MACSTR ", AID=%d", MAC2STR(connection->mac), connection->aid);
		break;
	}

	case WIFI_EVENT_AP_STADISCONNECTED:
	{
		wifi_event_ap_stadisconnected_t *connection = (wifi_event_ap_stadisconnected_t *)event_data;
		ESP_LOGI("WIFI_AP", "terminated connection: MAC=" MACSTR ", AID=%d", MAC2STR(connection->mac), connection->aid);
		break;
	}

	case WIFI_EVENT_AP_STOP:
	{
		ESP_LOGI("WIFI_AP", "Stopped");
		break;
	}

	default:
		break;
	}
}

/*
 * Initialize NVS storage for WiFi configuration
 */
void init_storage(void)
{
	ESP_ERROR_CHECK(nvs_flash_erase());
	ESP_ERROR_CHECK(nvs_flash_init());
}

/*
 * Initialize NETIF and WiFi
 */
void init_wifi()
{
	ESP_ERROR_CHECK(esp_netif_init());
	ESP_ERROR_CHECK(esp_wifi_init(&(wifi_init_config_t)WIFI_INIT_CONFIG_DEFAULT()));

	ESP_ERROR_CHECK(esp_event_loop_create_default());
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));

#if WIFI_ST_IS_CONFIGURED

	[[maybe_unused]] esp_netif_t *netif_st = esp_netif_create_default_wifi_sta();

	wifi_config_t wifi_st_config = {
		.sta = {
			.ssid     = WIFI_ST_SSID,
			.password = WIFI_ST_PASS,
		}
	};

#endif

#if WIFI_AP_IS_CONFIGURED

	[[maybe_unused]] esp_netif_t *netif_ap = esp_netif_create_default_wifi_ap();

	wifi_config_t wifi_ap_config = {
		.ap = {
			.ssid           = WIFI_AP_SSID,
			.password       = WIFI_AP_PASS,
			.authmode       = WIFI_AUTH_WPA3_PSK,
			.max_connection = 1,
		}
	};

#endif

#if WIFI_ST_IS_CONFIGURED && WIFI_AP_IS_CONFIGURED

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_st_config));

	// ESP_ERROR_CHECK(esp_netif_set_default_netif(netif_st));
	// ESP_ERROR_CHECK(esp_netif_napt_enable(netif_ap));

#elif WIFI_ST_IS_CONFIGURED

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_st_config));

#elif WIFI_AP_IS_CONFIGURED

	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));

#endif

	ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main(void)
{
	init_storage();
	init_wifi();
}
