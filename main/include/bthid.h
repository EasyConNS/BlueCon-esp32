#pragma once

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_hidd_api.h"
#include "esp_gap_bt_api.h"

void hidd_device_init(uint8_t *desc_list, int desc_list_len)
{
    static esp_hidd_app_param_t app_param;
    static esp_hidd_qos_param_t both_qos;
    app_param.name = "Wireless Gamepad";
    app_param.description = "Gamepad";
    app_param.provider = "Nintendo";
    app_param.subclass = 0x8;
    app_param.desc_list = desc_list;
    app_param.desc_list_len = desc_list_len;
    memset(&both_qos, 0, sizeof(esp_hidd_qos_param_t));
    esp_hid_device_register_app(&app_param, &both_qos, &both_qos);
    esp_bt_dev_set_device_name("Pro Controller");

    // const uint8_t *bdaddr_raw = esp_bt_dev_get_address();
}

#define BT_MODE_ON 1
#define BT_MODE_OFF 0
void set_bt_mode(int mode)
{
    const char *TAG = "change_bt_mode";
    if (mode == BT_MODE_ON)
    {
        ESP_LOGI(TAG, "setting bt connectable & discoverable");
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    }
    else
    {
        ESP_LOGI(TAG, "setting bt NON connectable & NON discoverable");
        esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
    }
}