/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"

// Libraries needed for the use of FreeRTOS.
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

// Library needed for GPIO functions.
#include "driver/gpio.h"

#define GPIO_LOW_STATE 0
#define GPIO_HIGH_STATE 1

#define BUTTON_GPIO 17

// print student identifier every second
void printStudentID(void* parameter)
{
    while (1)
    {
        printf("Nguyen Minh Linh - 2013630\n");
        printf("Ha Vinh Nguyen - 2011698\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

// print "ESP32" when the button is pressed.
void printESP32(void* parameter)
{
    esp_rom_gpio_pad_select_gpio(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_PULLDOWN);

    while (1)
    {
        if (gpio_get_level(BUTTON_GPIO) == GPIO_HIGH_STATE) printf("ESP32\n");
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
    
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(printStudentID, "printStudentID", 2048, NULL, 5, NULL);
    xTaskCreate(printESP32, "printESP32", 2048, NULL, 5, NULL);
    esp_restart();
}
