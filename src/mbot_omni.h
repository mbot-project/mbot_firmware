#ifndef MBOT_OMNI_H
#define MBOT_OMNI_H

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
#include "mbot_odometry_omni.h"

#include "config/mbot_omni_config.h"

// Some useful math definitions.
#define OMNI_MOTOR_ANGLE_LFT (-M_PI / 6.0f)   // Left wheel velocity angle (-30 degrees)
#define OMNI_MOTOR_ANGLE_BCK (M_PI / 2.0f)           // Back wheel velocity angle (90 degrees)
#define OMNI_MOTOR_ANGLE_RGT (-5.0 * M_PI / 6.0f)    // Right wheel velocity angle (-150 degrees)
#define SQRT3                   1.732050807568877

#endif /*MBOT_OMNI_H*/
