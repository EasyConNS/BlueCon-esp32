/*
  //  forked from https://github.com/NathanReeves/BlueCubeMod
*/

#include "main.h"

static SemaphoreHandle_t xSemaphore;
LED blink;

extern "C"
{
  void app_main(void);
}
void app_main()
{
  // init sdk:
  const char *TAG = "app_init";
  esp_err_t ret;
  ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));
  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  // esp_bt_mem_release(ESP_BT_MODE_BLE);
  if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK)
  {
    ESP_LOGE(TAG, "initialize controller failed: %s\n", esp_err_to_name(ret));
    return;
  }

  if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)
  {
    ESP_LOGE(TAG, "enable controller failed: %s\n", esp_err_to_name(ret));
    return;
  }

  if ((ret = esp_bluedroid_init()) != ESP_OK)
  {
    ESP_LOGE(TAG, "initialize bluedroid failed: %s\n", esp_err_to_name(ret));
    return;
  }

  if ((ret = esp_bluedroid_enable()) != ESP_OK)
  {
    ESP_LOGE(TAG, "enable bluedroid failed: %s\n", esp_err_to_name(ret));
    return;
  }

  xSemaphore = xSemaphoreCreateMutex();
  ESP_LOGI(TAG, "Hello from core %d\n", xPortGetCoreID());

  // main proc:
  // init hid device
  hidd_device_init(HID_PRO_CONTR,sizeof(HID_PRO_CONTR));
  //register callbacks
  // TODO
  // set bluetooth discoverable
  set_bt_mode(BT_MODE_ON);
  // blink led for notice
  blink.startBlink();
}
