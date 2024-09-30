# HCMUT - CO3053 - Embedded System

Practice embedded system programming with ESP32-WROOM-32 and ESP-IDF framework

-   [Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads)
-   [Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf)
-   [Documentation](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html)

Check [contributing guides](CONTRIBUTING.md) for more details.

## [Lab 1](lab1)

-   Introduction to ESP32, FreeRTOS, and ESP-IDF
-   Setup toolchain and dev environment
-   Build and flash given sample code

## [Lab 2](lab2)

-   Learn about ESP32 GPIO and FreeeRTOS task
-   Write a sample program that has 2 tasks:
    -   One polling task which will run every second
    -   One event based task which will run on GPIO

## [Lab 3](lab3)

-   FreeRTOS idle task
-   FreeRTOS task scheduling algorithms
-   Write sample programs for each scheduling algorithm

> [!IMPORTANT]
> Don't try to finish this lab as it is wrongly designed.
>
> -   All the instructions in document are from Vanilla FreeRTOS and only able to apply on Vanilla FreeRTOS.
> -   ESP-IDF, although based on FreeRTOS, has heavily modified it to support dual-core. And developers are not able to config its scheduling algorithm.

## [Lab 4](lab4)

-   Characteristics of queueing system
-   FreeRTOS queue management
-   Write a sample program using a queue with 1 source and 2 sinks

## [Lab 5](lab5)

-   FreeRTOS software timer
-   Software timer types, states, and behaviors
-   Write a sample program using 2 software timers sharing 1 callback function
