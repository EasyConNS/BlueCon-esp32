/*
  //  forked from https://github.com/NathanReeves/BlueCubeMod
*/

#include "main.h"

static uint8_t button_data[3] = {0x0};
static uint8_t lstick_data[3] = {0x00, 0x08, 0x80};
static uint8_t rstick_data[3] = {0x00, 0x08, 0x80};

static SemaphoreHandle_t xSemaphore;
TaskHandle_t SendingHandle = NULL;

void fill_reportdata(uint8_t left, STICK_STATE state)
{
  uint8_t *data = left ? lstick_data : rstick_data;

  switch (state)
  {
  case UP:
    data[0] = 0xd0;
    data[1] = 0xa7;
    data[2] = 0xfe;
    break;
  case DOWN:
    data[0] = 0xd0;
    data[1] = 0x07;
    data[2] = 0x02;
    break;
  case LEFT:
    data[0] = 0x20;
    data[1] = 0x00;
    data[2] = 0x7d;
    break;
  case RIGHT:
    data[0] = 0xea;
    data[1] = 0x0f;
    data[2] = 0x7d;
    break;
  default:
    data[0] = 0x00;
    data[1] = 0x08;
    data[2] = 0x80;
    break;
  }
}
void btnop(uint8_t *ops)
{
  memset(button_data, 0, 3);
  memcpy(button_data, ops, 3);
  uart_write_bytes(UART_NUM_0, "SP\t", 4);
}
STICK_STATE stickop(uint8_t op)
{
  switch (op)
  {
  case 0x1:
    uart_write_bytes(UART_NUM_0, "SU\t", 4);
    return UP;
    break;
  case 0x2:
    uart_write_bytes(UART_NUM_0, "SD\t", 4);
    return DOWN;
    break;
  case 0x3:
    uart_write_bytes(UART_NUM_0, "SL\t", 4);
    return LEFT;
    break;
  case 0x4:
    uart_write_bytes(UART_NUM_0, "SR\t", 4);
    return RIGHT;
    break;
  }
  uart_write_bytes(UART_NUM_0, "EE\t", 4);
  return RELEASE;
}
uint16_t fill_opcmd(uint8_t *rdata)
{
  uint8_t cmd = rdata[0];
  uint8_t op = rdata[1];
  switch (cmd)
  {
  case 0xa1: // buttons
    btnop(rdata + 1);
    break;
  case 0xb1: // left stick
    fill_reportdata(1, stickop(op));
    break;
  case 0xb2: // right stick
    fill_reportdata(0, stickop(op));
    break;
  case 0xef:
    if (op == 0x2d)
    {
      esp_restart();
    }
    break;
  default:
    //uart_write_bytes(UART_NUM_0, (const char *)a, 1);
    return 0;
    break;
  }
  return 1;
}
void empty_report()
{
  memset(button_data, 0, 3);
  fill_reportdata(0, RELEASE);
  fill_reportdata(1, RELEASE);
}
void uart_task()
{
  uint16_t report_count = 0;
  // Configure a temporary buffer for the incoming data
  uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
  uart_write_bytes(UART_NUM_0, "Ready", 6);
  while (true)
  {
    // Read data from the UART
    int len = uart_read_bytes(UART_NUM_0, data, BUF_SIZE, 20 / portTICK_RATE_MS);

    if (len == 1 && (data[0] == 0xa5))
    {
      // ping & pong
      uart_write_bytes(UART_NUM_0, (const char *)data, 1);
    }
    else if (len == 2)
    {
      // l r stick click
      xSemaphoreTake(xSemaphore, portMAX_DELAY);
      report_count = fill_opcmd(data);
      xSemaphoreGive(xSemaphore);
    }
    else if (len == 4)
    {
      //uint16_t *cmdop = (uint16_t *)data;
      // buttons
      xSemaphoreTake(xSemaphore, portMAX_DELAY);
      report_count = fill_opcmd(data);
      xSemaphoreGive(xSemaphore);
    }

    if (report_count > 0)
    {
      report_count--;
    }
    else
    {
      report_count = 0;
      empty_report();
    }

    // Write data back to the UART
    //uart_write_bytes(UART_NUM_0, (const char *) data, len);
    //vTaskDelay(50 / portTICK_RATE_MS);
    vTaskDelay(26);
  }
}
// http://blog.hypersect.com/interpreting-analog-sticks/
// https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering
void send_buttons(uint8_t timer)
{
  xSemaphoreTake(xSemaphore, portMAX_DELAY);
  report30[1] = timer;
  //buttons
  report30[3] = button_data[0];
  report30[4] = button_data[1];
  report30[5] = button_data[2];
  //encode left stick
  report30[6] = lstick_data[0];
  report30[7] = lstick_data[1];
  report30[8] = lstick_data[2];
  //encode right stick
  report30[9] = rstick_data[0];
  report30[10] = rstick_data[1];
  report30[11] = rstick_data[2];
  xSemaphoreGive(xSemaphore);

  send_report(sizeof(report30), report30);

  if (!paired)
  {
    vTaskDelay(150);
  }
  else
  {
    vTaskDelay(20);
  }
}

// sending bluetooth values every 15ms
void send_task(void *pvParameters)
{
  const char *TAG = "send_task";
  ESP_LOGI(TAG, "Sending hid reports on core %d\n", xPortGetCoreID());
  uint8_t timer = 0;
  while (true)
  {
    send_buttons(timer);
    timer += 1;
    if (timer == 255)
    {
      timer = 0;
    }
  }
}
void delete_send_task()
{
  if (SendingHandle != NULL)
  {
    vTaskDelete(SendingHandle);
    SendingHandle = NULL;
  }
}

void connected_cb()
{
  //turn off bt
  set_bt_mode(0);
  //clear blinking LED
  delete_blink_task();

  xSemaphoreTake(xSemaphore, portMAX_DELAY);
  connected = true;
  xSemaphoreGive(xSemaphore);

  //restart send_task
  delete_send_task();
  //restart uart_task
  delete_uart_task();

  //start solid
  xTaskCreatePinnedToCore((TaskFunction_t)send_task, "send_task", 2048, NULL, 1, &SendingHandle, 0);
  xTaskCreatePinnedToCore((TaskFunction_t)uart_task, "uart_task", 2048, NULL, 1, &UartHandle, 1);
}
void disconn_cb()
{
  //start blink
  create_blink_task();

  set_bt_mode(1);

  xSemaphoreTake(xSemaphore, portMAX_DELAY);
  connected = false;
  xSemaphoreGive(xSemaphore);
}

// callback for when hid host sends interrupt data
void intr_data_cb(uint16_t plen, uint8_t *pdata)
{
  ESP_LOGI("output report", "got an interrupt report from host, subcommand: %d  %d  %d Length: %d\n", pdata[10], pdata[11], pdata[12], plen);
}

void paired_cb(uint8_t light)
{
  ESP_LOGI("paired", "got light:%d\n", light);
}

bool init_()
{
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
    return false;
  }

  if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)
  {
    ESP_LOGE(TAG, "enable controller failed: %s\n", esp_err_to_name(ret));
    return false;
  }

  if ((ret = esp_bluedroid_init()) != ESP_OK)
  {
    ESP_LOGE(TAG, "initialize bluedroid failed: %s\n", esp_err_to_name(ret));
    return false;
  }

  if ((ret = esp_bluedroid_enable()) != ESP_OK)
  {
    ESP_LOGE(TAG, "enable bluedroid failed: %s\n", esp_err_to_name(ret));
    return false;
  }
  return true;
}
extern "C"
{
  void app_main(void);
}
void app_main()
{
  static bthid_callbacks_t param;
  init_uart();

  if (!init_())
  {
    return;
  }

  xSemaphore = xSemaphoreCreateMutex();

  hidd_register_app(sizeof(HID_PRO_CONTR), HID_PRO_CONTR);
  param.connected_cb = connected_cb;
  param.disconnected_cb = disconn_cb;
  param.paired_cb = paired_cb;
  param.intr_data_cb = intr_data_cb;
  hidd_device_init(&param, "Pro Controller");

  // turn on bt
  set_bt_mode(1);
  //start blinking
  create_blink_task();
}
