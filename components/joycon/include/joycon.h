#pragma once

#include "defs.h"
#include "input_report.h"
#include "output_report.h"

typedef void (*Sender_t)(uint16_t plen, uint8_t *pdata);

class JoyCon
{
    uint16_t datalen;
    input_report_t *replydata;
    Sender_t sender;
    player_t player;
    uint8_t timer = 0;
    void parse01(output_report_t *output); // Rumble and subcommand
    void parse10(output_report_t *output); // Rumble only
    void parse11(output_report_t *output); // Specific data from the NFC/IR MCU
    void send(uint16_t plen, uint8_t *pdata);

public:
    JoyCon(Sender_t s) : sender(s), player(PLAYER_0) {}
    ~JoyCon() {}
    void set_btaddr(const uint8_t *btaddr);
    uint8_t parse(uint16_t len, uint8_t *pdata);
    void send_buttons(button_t btn, stick_t l_stick, stick_t r_stick);
};
