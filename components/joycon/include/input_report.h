#pragma once

#include "defs.h"

#ifdef __cplusplus
extern "C" {
#endif

// input
#define INPUT_REPORT_STAND_SIZE 64
#define INPUT_REPORT_LARGE_SIZE 362
#define INPUT_PACKET_STAND_SIZE 36
#define INPUT_PACKET_EXTRA_SIZE 313

#pragma pack(1)
// 0x21 : 13~48(36)
typedef struct ReplyData
{
    uint8_t subcmd_ack;                        // 13
    uint8_t subcmd_id;                         // 14
    uint8_t data[INPUT_PACKET_STAND_SIZE - 2]; // 15~48(34)
} reply_data_t;

// 0x23 : 13~48(36)
typedef struct McuData
{
    uint8_t raw[INPUT_PACKET_STAND_SIZE];
} mcu_data_t;

// 0x30,0x31,0x32,0x33 : 13~48(36)
typedef struct ImuData
{
    accelerator_t acc_0;
    gyroscope_t gyro_0;
    accelerator_t acc_1;
    gyroscope_t gyro_1;
    accelerator_t acc_2;
    gyroscope_t gyro_2;
} imu_data_t;

typedef struct InputReport
{
    uint8_t id; // 0
    union {
        struct
        {
            // standard part [1:12],
            uint8_t timer;                       // 1
            controller_state_t controller_state; // 2
            controller_data_t controller_data;   // 3~11
            uint8_t vib_ack;                     // 12
            union {                              // 13~48
                reply_data_t reply;
                mcu_data_t mcu;
                imu_data_t imu;
            };
            // extra part [49:361]
            union {
                uint8_t nfc[INPUT_PACKET_EXTRA_SIZE];
                uint8_t ir[INPUT_PACKET_EXTRA_SIZE];
            };
        };
        // raw [1:362]
        uint8_t raw[INPUT_REPORT_STAND_SIZE];
    };
} input_report_t;
#pragma pack()

#ifdef __cplusplus
}
#endif
