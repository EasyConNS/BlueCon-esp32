#pragma once

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_hidd_api.h"
#include "esp_gap_bt_api.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "joycon.h"
#ifdef __cplusplus
}
#endif

typedef void (*connection_state_callback)(void);
typedef void (*paired_callback)(uint8_t light);
typedef void (*intr_data_callback)(uint16_t plen, uint8_t *pdata);
typedef struct
{
    connection_state_callback connected_cb;
    connection_state_callback disconnected_cb;
    paired_callback paired_cb;
    intr_data_callback intr_data_cb;
} bthid_callbacks_t;

void set_bt_mode(int mode);
void hidd_device_init(int desc_list_len, uint8_t *desc_list);
void hidd_register_app(bthid_callbacks_t *params);
void hidd_send_buttons(button_t btn, stick_t lstick, stick_t rstick);
