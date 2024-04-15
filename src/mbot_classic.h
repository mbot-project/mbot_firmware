#include "mbot.h"
#include <mbot/defs/mbot_params.h>

// Hardware Parameters
#define GEAR_RATIO              78.0
#define ENCODER_RES             40.0 // 40.0 for ROB103 encoders

// MBot Classic Parameters
#define DIFF_WHEEL_DIAMETER          0.0837
#define DIFF_WHEEL_RADIUS            0.04183
#define DIFF_BASE_RADIUS             0.07786
#define DIFF_MOTOR_LEFT_SLOT         0    // Left motor using M0 slot
#define DIFF_MOTOR_RIGHT_SLOT        1    // Right motor using M1 slot
#define UNUSED_DIFF_MOTOR_SLOT       2    // defined for mbot classic, 2 means M2 slot