#include "bthid.h"

void send_report(uint16_t len, uint8_t *data);
void connection_cb(esp_bd_addr_t bd_addr, esp_hidd_connection_state_t state);
void interrupt_data_cb(uint8_t report_id, uint16_t len, uint8_t *p_data);

static bool paired = false;
static bthid_callbacks_t bthid_cb;
static JoyCon joycon((Sender_t)send_report);

void send_report(uint16_t len, uint8_t *data)
{
    esp_hid_device_send_report(ESP_HIDD_REPORT_TYPE_INTRDATA, data[0], len - 1, data + 1);
}

// callback for hidd connection changes
void connection_cb(esp_bd_addr_t bd_addr, esp_hidd_connection_state_t state)
{
    switch (state)
    {
    case ESP_HIDD_CONN_STATE_CONNECTED:
        if (bthid_cb.connected_cb)
        {
            (bthid_cb.connected_cb)();
        }
        break;
    case ESP_HIDD_CONN_STATE_DISCONNECTED:
        if (bthid_cb.disconnected_cb)
        {
            (bthid_cb.disconnected_cb)();
        }
        paired = false;
        break;
    default:
        break;
    }
}

void interrupt_data_cb(uint8_t report_id, uint16_t len, uint8_t *p_data)
{
    p_data -= 1;
    len += 1;
    uint8_t tmp = p_data[0];
    p_data[0] = report_id;

    uint8_t rlen = joycon.parse(len, p_data);

    if (rlen == 48)
    {
        paired = true;
        if (bthid_cb.paired_cb)
        {
            (bthid_cb.paired_cb)(p_data[11]);
        }
    }

    p_data[0] = tmp;
}

void set_bt_mode(int mode)
{
    const char *TAG = "change_bt_mode";
    if (mode == 1)
    {
        ESP_LOGI(TAG, "setting bt connectable & discoverable");
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    }
    else
    {
        ESP_LOGI(TAG, "setting bt non connectable & discoverable");
        esp_bt_gap_set_scan_mode(ESP_BT_NON_CONNECTABLE, ESP_BT_NON_DISCOVERABLE);
    }
}

void hidd_device_init(int desc_list_len, uint8_t *desc_list)
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

    const uint8_t *bdaddr_raw = esp_bt_dev_get_address();
    joycon.set_btaddr(bdaddr_raw);
}

void hidd_register_app(bthid_callbacks_t *params)
{
    bthid_cb.connected_cb = params->connected_cb;
    bthid_cb.disconnected_cb = params->disconnected_cb;
    bthid_cb.paired_cb = params->paired_cb;
    bthid_cb.intr_data_cb = params->intr_data_cb;
    //  Must be done after esp_hid_device_register_app
    static esp_hidd_callbacks_t callbacks;
    callbacks.connection_state_cb = connection_cb;
    callbacks.intr_data_cb = interrupt_data_cb;
    esp_hid_device_init(&callbacks);
}

void hidd_send_buttons(button_t btn, stick_t lstick, stick_t rstick){
    joycon.send_buttons(btn, lstick, rstick);
}
