#pragma once

#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#include "nvs.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "LED.h"
#include "uart_cmd.h"
#include "bthid.h"
#include "descriptors.h"

typedef enum
{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT,
    RELEASE
} STICK_STATE;

static bool connected = false;
static bool paired = false;
