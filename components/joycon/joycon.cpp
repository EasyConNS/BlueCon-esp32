#include "joycon.h"

#include "reply_pack.h" // deprecated

uint8_t JoyCon::parse(uint16_t len, uint8_t *pdata)
{
    this->datalen = 0;
    output_report_t *outdata = (output_report_t *)pdata;
    switch (outdata->id)
    {
    case OUTPUT_REPORT_CMD:
        this->parse01(outdata);
        break;
    case OUTPUT_REPORT_RUM:
        this->parse10(outdata);
        break;
    case OUTPUT_REPORT_MCU:
        this->parse11(outdata);
        break;
    }
    return outdata->subcmd.subid;
}

void JoyCon::parse01(output_report_t *outdata)
{
    switch (outdata->subcmd.subid)
    {
    case 0x2:
        this->send(sizeof(reply02), reply02);
        break;
    case 0x3:
        this->send(sizeof(reply03), reply03);
        break;
    case 0x4:
        this->send(sizeof(reply04), reply04);
        break;
    case 0x8:
        this->send(sizeof(reply08), reply08);
        break;
    case 0x10: // spi read
        ESP_LOGI("spi read", "len:%d@0x%04x", outdata->subcmd_10.length, outdata->subcmd_10.address);
        switch (outdata->subcmd_10.address)
        {
        case 0x6000:
            this->send(sizeof(reply1060), reply1060);
            break;
        case 0x6020:
            this->send(sizeof(reply1020), reply1020);
            break;
        case 0x603d:
            this->send(sizeof(reply103D), reply103D);
            break;
        case 0x6050:
            this->send(sizeof(reply1050), reply1050);
            break;
        case 0x6080:
            this->send(sizeof(reply1080), reply1080);
            break;
        case 0x6098:
            this->send(sizeof(reply1098), reply1098);
            break;
        case 0x8010:
            this->send(sizeof(reply1010), reply1010);
            break;
        default:
            break;
        }
        break;
    case 0x21:
        if (outdata->subcmd_21.mcucmd == MCU_CMD_SET_MODE)
        {
            this->send(sizeof(reply2100), reply2100);
        }
        break;
    case 0x30:
        this->send(sizeof(reply3001), reply3001);
        break;
    case 0x40:
        this->send(sizeof(reply4001), reply4001);
        break;
    case 0x48:
        if (outdata->subcmd_48.enable_vibration == 1)
        {
            this->send(sizeof(reply4801), reply4801);
        }
        break;
    default:
        break;
    }
}
void JoyCon::parse10(output_report_t *outdata)
{
}
void JoyCon::parse11(output_report_t *outdata)
{
}
void JoyCon::send(uint16_t plen, uint8_t *pdata)
{
    if (this->sender)
    {
        this->sender(plen, pdata);
    }
}
void JoyCon::set_btaddr(const uint8_t *btaddr)
{
    for (int i = 0; i < 6; i++)
    {
        reply02[19 + i] = btaddr[i];
    }
    mac_address_le_t m = mac_address_le(reply02 + 19);
    ESP_LOGI("bt address", "%s", mac_address_to_str(m));
}
void JoyCon::set_buttons(button_t *btn, stick_t *l_stick, stick_t *r_stick) {}
