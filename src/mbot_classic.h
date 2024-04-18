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

#if CLASSIC_VERSION == 10
/****************** CLASSIC Version 1.0*******************/
// Hardware Parameters
#define GEAR_RATIO              78.0
#define ENCODER_RES             40.0 // 40.0 for ROB103 encoders

// MBot Classic Parameters
#define DIFF_WHEEL_DIAMETER          0.0837
#define DIFF_WHEEL_RADIUS            0.04183
#define DIFF_BASE_RADIUS             0.07786
#define MOT_R                        1   // Right motor slot
#define MOT_L                        0   // Left motor slot
#define MOT_UNUSED                   2 // Unused motor slot


#elif CLASSIC_VERSION == 20
/****************** CLASSIC Version 2.0*******************/


#endif /*CLASSIC_VERSION*/

#endif /*MBOT_CLASSIC_H*/