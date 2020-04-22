#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include "blink.h"
#ifdef __cplusplus
}
#endif

LED blink;
TaskHandle_t BlinkHandle = NULL;

//LED blink
void startBlink()
{
  while (1)
  {
    blink.set_level(1);
    vTaskDelay(150);
    blink.set_level(0);
    vTaskDelay(150);
    blink.set_level(1);
    vTaskDelay(150);
    blink.set_level(0);
    vTaskDelay(1000);
  }
  vTaskDelete(NULL);
}

void create_blink_task()
{
  xTaskCreate((TaskFunction_t)startBlink, "blink_task", 1024, NULL, 5, &BlinkHandle);
}

void delete_blink_task()
{
  if (BlinkHandle != NULL)
  {
    vTaskDelete(BlinkHandle);
    BlinkHandle = NULL;
    // turn led on
    blink.set_level(1);
  }
}
