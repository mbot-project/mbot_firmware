#ifndef MBOT_CLASSIC_H
#define MBOT_CLASSIC_H

#include <pico/stdlib.h>
#include <pico/mutex.h>
#include <pico/time.h>
#include <hardware/gpio.h>
#include <mbot/motor/motor.h>
#include <mbot/encoder/encoder.h>
#include <mbot/motor/motor.h>
#include <mbot/defs/mbot_pins.h>
#include <mbot/defs/mbot_params.h>
#include <mbot/fram/fram.h>
#include <mbot/imu/imu.h>
#include <mbot/utils/utils.h>
#include <rc/math/filter.h>
#include <rc/mpu/mpu.h>
#include <math.h>
#include <inttypes.h>
#include "mbot_print.h"
#include "mbot_comms.h"
#include "mbot_odometry.h"

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

#endif