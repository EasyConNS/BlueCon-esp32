#pragma once

#include "device.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum McuCmd {
    MCU_CMD_SET_MODE = 0x21, // 0x21
    MCU_CMD_WRITE = 0x23,    // 0x23
} mcu_cmd_t;

typedef enum McuSubcmd {
    MCU_SET_IR_MODE = 0x1,
    MCU_SET_IR_REG = 0x4,
} mcu_subcmd_t;

typedef enum McuMode {
    MCU_MODE_STANDBY = 0x1, // 1: Standby
    MCU_MODE_NFC = 0x4,     // 4: NFC
    MCU_MODE_IR = 0x5,      // 5: IR
    MCU_MODE_INIT = 0x6,    // 6: Initializing/FW Update?
} mcu_mode_t;

typedef enum McuState {
    MCU_STATE_SUSPEND = 0,
    MCU_STATE_RESUME,
    MCU_STATE_UPDATE,
} mcu_state_t;

#ifdef __cplusplus
}
#endif
