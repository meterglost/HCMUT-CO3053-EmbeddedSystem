#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include <stdio.h>

typedef struct
{
	char *text;
	uint8_t counter;
} TimerData_t;

void vTimerCallback(TimerHandle_t xTimer)
{
	TimerData_t *timerData = (TimerData_t *)pvTimerGetTimerID(xTimer);

	TickType_t xTimeNow = xTaskGetTickCount();

	if (timerData->counter > 0)
	{
		printf("%4u: %s %2u\n", xTimeNow, timerData->text, timerData->counter);
		timerData->counter--;
	}

	if (timerData->counter == 0)
	{
		xTimerStop(xTimer, 0);
	}
}

void app_main(void)
{
	TimerData_t *timerData1 = (TimerData_t *)malloc(sizeof(TimerData_t));
	timerData1->text        = "ahihi";
	timerData1->counter     = 10;

	TimerData_t *timerData2 = (TimerData_t *)malloc(sizeof(TimerData_t));
	timerData2->text        = "ihaha";
	timerData2->counter     = 5;

	if (timerData1 == NULL || timerData2 == NULL)
	{
		printf("Failed to allocate memory for timer data\n");
		goto cleanupData;
	}

	TimerHandle_t timer1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(2000), pdTRUE, timerData1, vTimerCallback);
	TimerHandle_t timer2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(3000), pdTRUE, timerData2, vTimerCallback);

	if (timer1 == NULL || timer2 == NULL)
	{
		printf("Failed to create timer\n");
		goto cleanupTimer;
	}

	BaseType_t xTimer1Started = xTimerStart(timer1, 0);
	BaseType_t xTimer2Started = xTimerStart(timer2, 0);

	if (xTimer1Started != pdPASS || xTimer2Started != pdPASS)
	{
		printf("Failed to start timer\n");
		goto cleanupTimer;
	}

	while (xTimerIsTimerActive(timer1) == pdTRUE || xTimerIsTimerActive(timer2) == pdTRUE || timerData1->counter > 0 || timerData2->counter > 0)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

	printf("Timers have finished\n");

cleanupTimer:
	xTimerDelete(timer1, 0);
	xTimerDelete(timer2, 0);

cleanupData:
	free(timerData1);
	free(timerData2);
}
