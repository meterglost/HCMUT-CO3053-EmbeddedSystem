#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_timer.h"

#include <stdio.h>

void vSampleTask()
{
	printf("\r");
}

void vTaskMont(void *pvParameters)
{
	uint32_t *counter = (uint32_t *)pvParameters;

	for (int times = 20; times > 0; --times)
	{
		TickType_t ticks = xTaskGetTickCount();
		printf("| %20ld | %20ld | %20ld |\n", ticks - 1, counter[0], counter[1]);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

	vTaskDelete(NULL);
}

void vTaskEvnt(void *pvParameters)
{
	vTaskDelay(pdMS_TO_TICKS(5000));

	for (int i = 0; i < 100000; ++i)
	{
		(*(uint32_t *)pvParameters)++;
		vSampleTask();
	}

	vTaskDelete(NULL);
}

void vTaskCont(void *pvParameters)
{
	for (int i = 0; i < 100000; ++i)
	{
		(*(uint32_t *)pvParameters)++;
		vSampleTask();
	}

	vTaskDelete(NULL);
}

void app_main(void)
{
	uint32_t counter[2] = { 0, 0 };

	TaskHandle_t taskMont = NULL, taskCont = NULL, taskEvnt = NULL;

	const char *padline = "--------------------";

	printf("Preemption: %s\n", configUSE_PREEMPTION ? "yes" : "no");
	printf("Time slicing: %s\n", configUSE_TIME_SLICING ? "yes" : "no");

	printf("+ %20s + %20s + %20s +\n", padline, padline, padline);

	printf("| %20s | %20s | %20s |\n", "Ticks", "Task1 Counter", "Task2 Counter");

	printf("+ %20s + %20s + %20s +\n", padline, padline, padline);

	xTaskCreatePinnedToCore(vTaskMont, "Task Mont", 2048, &counter, 1, &taskMont, 0);

	xTaskCreatePinnedToCore(vTaskCont, "Task Cont", 2048, &counter[0], 1, &taskCont, 1);

	xTaskCreatePinnedToCore(vTaskEvnt, "Task Evnt", 2048, &counter[1], 2, &taskEvnt, 1);

	while (eTaskGetState(taskMont) != eDeleted)
		vTaskDelay(pdMS_TO_TICKS(1000));

	printf("+ %20s + %20s + %20s +\n", padline, padline, padline);

	if (eTaskGetState(taskCont) != eDeleted)
		vTaskDelete(taskCont);

	if (eTaskGetState(taskEvnt) != eDeleted)
		vTaskDelete(taskEvnt);
}
