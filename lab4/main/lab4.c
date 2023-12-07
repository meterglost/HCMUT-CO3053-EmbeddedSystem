#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_random.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/queue.h"

typedef struct {
	uint8_t id;
	uint8_t source;
	uint32_t value;
} Data_t;

#define xTicksToWait pdMS_TO_TICKS(100)

QueueHandle_t xQueueTask1, xQueueTask2;

void vTaskReception(Data_t *request)
{
	BaseType_t xTaskStatus;
	QueueHandle_t xQueue;

	switch (request->source)
	{
	case 1:
		xQueue = xQueueTask1;
		break;

	case 2:
		xQueue = xQueueTask2;
		break;

	default:
		xQueue = NULL;
		break;
	}

	if (xQueue != NULL)
		xTaskStatus = xQueueSendToBack(xQueue, request, xTicksToWait);
	else
		xTaskStatus = pdFAIL;

	if (xTaskStatus == pdPASS)
		ESP_LOGI("Reception", "request %d is put on queue %d", request->id, request->source);
	else
		ESP_LOGE("Reception", "request %d is rejected", request->id);
}

void vTask1(void *pvParameters)
{
	Data_t request;
	BaseType_t xTaskStatus;

	for ( ; ; )
	{
		xTaskStatus = xQueueReceive(xQueueTask1, &request, xTicksToWait);

		if (xTaskStatus == pdFAIL) continue;

		if (request.source == 1)
			ESP_LOGI("Queue 1", "handled request %d: value = %ld", request.id, request.value);
		else
			ESP_LOGE("Queue 1", "rejected request %d", request.id);
	}

	vTaskDelete(NULL);
}

void vTask2(void *pvParameters)
{
	Data_t request;
	BaseType_t xTaskStatus;

	for ( ; ; )
	{
		xTaskStatus = xQueueReceive(xQueueTask2, &request, xTicksToWait);

		if (xTaskStatus == pdFAIL) continue;

		if (request.source == 2)
			ESP_LOGI("Queue 2", "handled request %d: value = %ld", request.id, request.value);
		else
			ESP_LOGE("Queue 2", "rejected request %d", request.id);
	}

	vTaskDelete(NULL);
}

void vCreateRequest(void *pvParameters)
{
	for (uint8_t i = 1; i <= 50; ++i)
	{
		Data_t *request = &(Data_t){ .id = i, .source = esp_random() % 3 + 1, .value = esp_random() };

		printf("Request %d is created: source = %d, value = %ld\r\n", request->id, request->source, request->value);
		vTaskReception(request);
	}

	vTaskDelete(NULL);
}

void app_main(void)
{
	xQueueTask1 = xQueueCreate(5, sizeof(Data_t));
	xQueueTask2 = xQueueCreate(5, sizeof(Data_t));

	if (xQueueTask1 == NULL || xQueueTask2 == NULL)
	{
		printf("=> Failed to create queue !\r\n");
		return;
	}

	xTaskCreate(vCreateRequest, "Create Request", 1024 * 5, NULL, 2, NULL);
	xTaskCreate(vTask1, "Task 1", 1024 * 5, NULL, 1, NULL);
	xTaskCreate(vTask2, "Task 2", 1024 * 5, NULL, 1, NULL);
}
