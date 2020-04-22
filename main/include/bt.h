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

// Standard full mode
static uint8_t report30[] = {
    0x30, // report ID Standard full mode
    /* input reports with IMU data instead of subcommand replies
       Pushes current state @60Hz, or @120Hz if Pro Controller */
    /* 0x3F for This input packet is pushed to the host when a button is pressed or released
       and provides the "normal controller" interface for the OS.*/
    // 0x21 for Standard input reports used for subcommand replies
    // 0x31 for NFC/IR MCU mode
    0x00, // Timer
    0x80, // battery level
    0,    //but1
    0,    //but2
    0,    //but3
    0,    //Lstick
    0x08, //Ls
    0x80, //Ls
    0,    //Rstick
    0x08, //Rs
    0x80, //Rs
    0x08};

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

static void _send_report_fixed(uint8_t reportid, uint16_t len, uint8_t *pdata)
{
    esp_hid_device_send_report(ESP_HIDD_REPORT_TYPE_INTRDATA, reportid, len, pdata);
}
static void send_report(uint16_t len, uint8_t *data)
{
    _send_report_fixed(data[0], len - 1, data + 1);
}

static bool paired = false;
static bthid_callbacks_t *bthid_cb;

void connection_cb(esp_bd_addr_t bd_addr, esp_hidd_connection_state_t state);
void raw_intr_data_cb(uint8_t report_id, uint16_t len, uint8_t *p_data);
void set_bt_mode(int mode);
void hidd_register_app(int desc_list_len, uint8_t *desc_list);
void hidd_device_init(bthid_callbacks_t *params, const char *device_name);
