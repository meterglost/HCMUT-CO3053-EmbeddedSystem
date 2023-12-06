#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"
#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

TimerHandle_t timer1, timer2;

void vTimerCallback(TimerHandle_t xTimer)
{
	uint32_t counter = (uint32_t)pvTimerGetTimerID(xTimer);
	++counter;
	vTimerSetTimerID(xTimer, (void *)counter);

	TickType_t xTimeNow = xTaskGetTickCount();

	if (xTimer == timer1)
	{
		printf("%7ld: ahihi %7ld\n", xTimeNow, counter);
		if (counter == 10) xTimerStop(xTimer, 0);
	}
	else if (xTimer == timer2)
	{
		printf("%7ld: ihaha %7ld\n", xTimeNow, counter);
		if (counter == 5) xTimerStop(xTimer, 0);
	}
}

void app_main(void)
{
	timer1 = xTimerCreate("Timer 1", pdMS_TO_TICKS(2000UL), pdTRUE, 0, vTimerCallback);
	timer2 = xTimerCreate("Timer 2", pdMS_TO_TICKS(3000UL), pdTRUE, 0, vTimerCallback);

	if (timer1 == NULL || timer2 == NULL)
	{
		printf("Failed to create timer\n");
		return;
	}

	xTimerStart(timer1, 0);
	xTimerStart(timer2, 0);
}
