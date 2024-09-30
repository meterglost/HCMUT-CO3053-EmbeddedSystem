#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_system.h"

#include <stdio.h>

// print student identifier every second
void printStudentID(void *parameter)
{
	for (;;)
	{
		printf("<Student_ID>\n");
		vTaskDelay(pdMS_TO_TICKS(3000));
	}
}

// print "ESP32" when the button is pressed.
void printESP32(void *parameter)
{
	const gpio_num_t BUTTON_GPIO = GPIO_NUM_17;

	esp_rom_gpio_pad_select_gpio(BUTTON_GPIO);
	gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
	gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_PULLDOWN);

	enum GPIO_STATE
	{
		GPIO_LOW_STATE,
		GPIO_HIGH_STATE
	};

	for (;;)
	{
		if (gpio_get_level(BUTTON_GPIO) == GPIO_HIGH_STATE)
			printf("ESP32\n");
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void app_main(void)
{
	xTaskCreate(printStudentID, "printStudentID", 2048, NULL, 1, NULL);
	xTaskCreate(printESP32, "printESP32", 2048, NULL, 1, NULL);
}
