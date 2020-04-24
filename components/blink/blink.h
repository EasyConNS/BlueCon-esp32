#pragma once

#include "driver/gpio.h"

#define BLINK_GPIO GPIO_NUM_2

class LED
{
    TaskHandle_t BlinkHandle = NULL;
    gpio_num_t led_num;
    void blinkTask();

public:
    LED();
    void startBlink();
    void stop();
    void turnON();
    void turnOFF();
};