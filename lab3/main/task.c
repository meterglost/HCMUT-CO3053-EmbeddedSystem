#include <stdio.h>
#include "task.h"

void vTaskEvent(void * pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(100));

    for ( int i = 0; i < 500000; ++i )
    {
        ++((task_param *) pvParameters)->counter;
    }

    ((task_param *) pvParameters)->counter = 0;

    // if (!configUSE_PREEMPTION) {
    //     taskYIELD();
    // }

    vTaskDelete(NULL);
}

void vTaskContinuous(void * pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for ( int i = 0; i < 10; ++i )
    {
        if (configUSE_PREEMPTION) {
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
        }

        for ( int i = 0; i < 1000000; ++i )
        {
            ++((task_param *) pvParameters)->counter;
        }

        ((task_param *) pvParameters)->counter = 0;

        // if (!configUSE_PREEMPTION) {
        //     taskYIELD();
        // }
    }

    vTaskDelete(NULL);
}
