#pragma once

#include "driver/gpio.h"

#define BLINK_GPIO GPIO_NUM_2

class LED
{
    gpio_num_t led_num;
    public:
    LED();
    void set_level(int mode);
};