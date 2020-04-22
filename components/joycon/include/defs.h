#pragma once

#include "device.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FLASH_MEM_SIZE 0x80000 // 524288 bytes
#define FLASH_MEM_STEP 0x1d

typedef enum Battery
{
    BATT_EMPTY = 0,    // 0000
    BATT_CHARGING = 1, // 0001
    BATT_CRITICAL = 2, // 0010
    BATT_LOW = 4,      // 0100
    BATT_MEDIUM = 6,   // 0110
    BATT_FULL = 8,     // 1000
} battery_t;

typedef enum Category
{
    PRO_GRIP = 0,
    HANDHELD = 1,     // from yuzu
    JoyCon_DUAL = 2,  // from yuzu
    JoyCon_LEFT = 3,  //
    JoyCon_RIGHT = 4, //
    POKE_BALL = 6,    // from yuzu
} category_t;

typedef enum Power
{
    SELF = 0,
    SWITCH = 1,
} power_t;

typedef enum Player
{
#define _PLAYER(n) ((0x1 << (n)) - 1)
    PLAYER_0 = _PLAYER(0),
    PLAYER_1 = _PLAYER(1),
    PLAYER_2 = _PLAYER(2),
    PLAYER_3 = _PLAYER(3),
    PLAYER_4 = _PLAYER(4),
} player_t;
#define Player_(n) player_t(_PLAYER(n))

typedef enum ButtonState
{
    BTNIDLE = 0,
    BTNPRESSED = 1,
} button_state_t;

#pragma pack(1)
typedef struct Button
{
    union {
        struct
        {
            // Right byte
            button_state_t Y : 1;
            button_state_t X : 1;
            button_state_t B : 1;
            button_state_t A : 1;
            button_state_t RSR : 1; // right Joy-Con only
            button_state_t RSL : 1; // right Joy-Con only
            button_state_t R : 1;
            button_state_t ZR : 1;
            // Shared byte
            button_state_t MINUS : 1;
            button_state_t PLUS : 1;
            button_state_t RS : 1;
            button_state_t LS : 1;
            button_state_t HOME : 1;
            button_state_t CAPTURE : 1;
            button_state_t NA : 1;
            button_state_t CAHRGING_GRIP : 1;
            // Left byte
            button_state_t DPAD_DOWN : 1;
            button_state_t DPAD_UP : 1;
            button_state_t DPAD_RIGHT : 1;
            button_state_t DPAD_LEFT : 1;
            button_state_t LSR : 1; // left Joy-Con only
            button_state_t LSL : 1; // left Joy-Con only
            button_state_t L : 1;
            button_state_t ZL : 1;
        };
        struct
        {
            /*
Byte	Bit     x01     x02     x04     x08     x10     x20     x40     x80
3 (Right)       Y       X       B       A       SR      SL      R      ZR
4 (Shared)      Minus   Plus    RStick  LStick  Home    Capture --     Charging Grip
5 (Left)        Down    Up      Right   Left    SR      SL      L      ZL
*/
            uint8_t left;
            uint8_t shared;
            uint8_t right;
        };
    };
} button_t;

typedef struct Stick
{
    union {
        struct
        {
            uint16_t X : 12;
            uint16_t Y : 12;
        };
        uint8_t raw[3];
    };
} stick_t;

typedef struct Accelerator
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} accelerator_t;

typedef struct Gyroscope
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} gyroscope_t;

typedef struct ControllerState
{
    power_t power : 1;
    category_t category : 3;
    battery_t battery : 4;
} controller_state_t;

typedef struct ControllerData
{
    button_t button;
    stick_t left_stick;
    stick_t right_stick;
} controller_data_t;

typedef struct ControllerColor
{
    uint8_t body_color[3];
    uint8_t button_color[3];
    uint8_t left_grip_color[3];
    uint8_t right_grip_color[3];
    uint8_t _;
} controller_color_t;
#pragma pack()

#ifdef __cplusplus
}
#endif
