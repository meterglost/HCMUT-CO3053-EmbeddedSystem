/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include <inttypes.h>
#include <stdio.h>

void app_main(void)
{
	esp_chip_info_t chip_info;
	esp_chip_info(&chip_info);

	unsigned int major_rev = chip_info.revision / 100;
	unsigned int minor_rev = chip_info.revision % 100;

	printf("This is %s chip with %u CPU core(s), silicon revision v%u.%u\n", CONFIG_IDF_TARGET, chip_info.cores, major_rev, minor_rev);

	if (chip_info.features & CHIP_FEATURE_WIFI_BGN)
		printf("    - 2.4GHz WiFi\n");

	if (chip_info.features & CHIP_FEATURE_BLE)
		printf("    - Bluetooth LE\n");
	else if (chip_info.features & CHIP_FEATURE_BT)
		printf("    - Bluetooth Classic\n");

	if (chip_info.features & CHIP_FEATURE_IEEE802154)
		printf("    - IEEE 802.15.4 (Zigbee/Thread)\n");

	unsigned int flash_size;
	if (esp_flash_get_physical_size(NULL, &flash_size) == ESP_OK)
	{
		if (chip_info.features & CHIP_FEATURE_EMB_FLASH)
			printf("    - %u Embedded Flash\n", flash_size / (unsigned int)(1024 * 1024));
		else
			printf("    - %u External Flash\n", flash_size / (unsigned int)(1024 * 1024));
	}

	for (int i = 10; i > 0; i--)
	{
		printf("\rRestarting in %d seconds...", i);
		fflush(stdout);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

	esp_restart();
}
