#include "blink.h"

LED::LED()
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       muxed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}
void LED::blinkTask()
{
    while (1)
    {
        this->turnON();
        vTaskDelay(150);
        this->turnOFF();
        vTaskDelay(300);
        this->turnON();
        vTaskDelay(150);
        this->turnOFF();
        vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}
void LED::startBlink()
{
    xTaskCreate((TaskFunction_t)this->blinkTask, "blink_task", 1024, NULL, 5, &this->BlinkHandle);
}
void LED::stop()
{
    if (this->BlinkHandle != NULL)
    {
        vTaskDelete(this->BlinkHandle);
        this->BlinkHandle = NULL;
    }
}

void LED::turnON()
{
    gpio_set_level(BLINK_GPIO, 1);
}
void LED::turnOFF()
{
    gpio_set_level(BLINK_GPIO, 0);
}
