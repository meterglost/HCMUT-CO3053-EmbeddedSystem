#ifndef __TASK_H__
#define __TASK_H__

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "FreeRTOSConfig.h"

typedef struct {
    uint32_t counter;
} task_param;

// Delay a period of time then increase the counter.
void vTaskEvent(void * pvParameters);

// Repeatly increase the counter and delay a few milliseconds.
void vTaskContinuous(void * pvParameters);

#endif
