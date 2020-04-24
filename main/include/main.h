#pragma once

#include "nvs.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "bthid.h"
#include "descriptors.h"

extern "C"
{
#include "blink.h"
}
