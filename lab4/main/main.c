#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_random.h"

#include <stdio.h>

typedef struct
{
	uint32_t id;
	uint32_t value;
} Request_t;

QueueHandle_t xQueueRequest;

uint32_t rand_int(uint32_t min, uint32_t max)
{
	return esp_random() % (max - min + 1) + min;
}

void vTaskReception(void *pvParameters)
{
	for (uint32_t idx = 1; idx < 50; ++idx)
	{
		Request_t request = { .id = idx, .value = rand_int(1, 9) };
		if (xQueueSendToBack(xQueueRequest, &request, pdMS_TO_TICKS(100)) == pdPASS)
			printf("Request %ld is queued: value = %ld\n", request.id, request.value);
		vTaskDelay(pdMS_TO_TICKS(rand_int(100, 300)));
	}

	vTaskDelete(NULL);
}

void vTaskFunctional(void *pvParameters)
{
	for (;;)
	{
		Request_t request;
		if (xQueueReceive(xQueueRequest, &request, pdMS_TO_TICKS(100)) == pdPASS)
		{
			printf("Request %ld is handled by %s: value = %ld\n", request.id, pcTaskGetName(NULL), request.value);
			vTaskDelay(pdMS_TO_TICKS(rand_int(700, 900)));
		}
	}
}

void app_main(void)
{
	xQueueRequest = xQueueCreate(5, sizeof(Request_t));

	if (xQueueRequest == NULL)
	{
		printf("Failed to create queue!");
		return;
	}

	if (xTaskCreatePinnedToCore(vTaskReception, "Reception Task", 1024 * 2, NULL, 1, NULL, 0) != pdPASS)
	{
		printf("Failed to create Reception Task!\n");
		return;
	}

	if (xTaskCreatePinnedToCore(vTaskFunctional, "Functional Task 1", 1024 * 2, NULL, 1, NULL, 1) != pdPASS)
	{
		printf("Failed to create Functional Task 1!\n");
		return;
	}

	if (xTaskCreatePinnedToCore(vTaskFunctional, "Functional Task 2", 1024 * 2, NULL, 1, NULL, 1) != pdPASS)
	{
		printf("Failed to create Functional Task 2!\n");
		return;
	}

	if (xTaskCreatePinnedToCore(vTaskFunctional, "Functional Task 3", 1024 * 2, NULL, 1, NULL, 1) != pdPASS)
	{
		printf("Failed to create Functional Task 3!\n");
		return;
	}
}
