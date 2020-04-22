#pragma once

#include <stdio.h>
#include <stdint.h>

#pragma pack(1)

//device
typedef struct MacAddress
{
    uint8_t _0;
    uint8_t _1;
    uint8_t _2;
    uint8_t _3;
    uint8_t _4;
    uint8_t _5;
} mac_address_t;
typedef mac_address_t mac_address_le_t;
#define mac_address_le(raw) \
    (mac_address_t) { (raw)[0], (raw)[1], (raw)[2], (raw)[3], (raw)[4], (raw)[5] }
#define MAC_ADDRESS_INIT \
    {                    \
        0, 0, 0, 0, 0, 0 \
    }

#pragma pack()

#define mac_address_fmt "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX"
#define mac_address_to_str(address)                                                                            \
    ({                                                                                                         \
        char mac[18] = {'\0'};                                                                                 \
        sprintf(mac, mac_address_fmt, address._0, address._1, address._2, address._3, address._4, address._5); \
        mac;                                                                                                   \
    })
