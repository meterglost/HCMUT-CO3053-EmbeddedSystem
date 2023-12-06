/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "esp_system.h"

#include "task.h"

volatile uint32_t ulIdleCounter = 0UL;
void vApplicationIdleHook(void) { ulIdleCounter++; }

static task_param task1_param = { .counter = 0 };
static task_param task2_param = { .counter = 0 };
// static task_param task3_param = { .counter = 0 };

void vMonitor(void * pvParameters)
{
    printf("Preemption: %s\n", configUSE_PREEMPTION ? "yes" : "no");
    printf("Time slicing: %s\n", configUSE_TIME_SLICING ? "yes" : "no");

    const char * padline = "--------------------";

    printf("+ %20s + %20s + %20s +\n", padline, padline, padline);

    printf("| %20s | %20s | %20s |\n", "Idle Counter", "Task1 Counter", "Task2 Counter");

    printf("+ %20s + %20s + %20s +\n", padline, padline, padline);

    for ( int times = 50; times > 0; --times )
    {
        printf("| %20ld | %20ld | %20ld |\n", ulIdleCounter, task1_param.counter, task2_param.counter);
        for ( int i = 0; i < 1000; ++i ) { }
    }

    printf("+ %20s + %20s + %20s +\n", padline, padline, padline);

    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreatePinnedToCore(vMonitor, "Monitor", 2048, NULL, 0, NULL, 0);
    xTaskCreatePinnedToCore(vTaskEvent, "Task 1", 2048, &task1_param, 5, NULL, 1);
    xTaskCreatePinnedToCore(vTaskContinuous, "Task 2", 2048, &task2_param, 3, NULL, 1);
    // xTaskCreatePinnedToCore(vTaskContinuous, "Task 3", 2048, &task3_param, 1, NULL, 1);
}
