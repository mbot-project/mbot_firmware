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
#include "mbot_odometry.h"

#if OMNI_VERSION == 10
/****************** OMNI Version 1.0*******************/
// Hardware Parameters
#define GEAR_RATIO              78.0
#define ENCODER_RES             40.0  
// MBot Omni Parameters
#define OMNI_BASE_RADIUS        0.10843     // Radius of base, from center of base to middle of omni wheels
                                            // Base radius to outer surface on wheel is 0.1227
#define OMNI_WHEEL_RADIUS       0.048       // 0.050 for old wheels
#define OMNI_MOTOR_ANGLE_LFT (-M_PI / 6.0f)   // Left wheel velocity angle (-30 degrees)
#define OMNI_MOTOR_ANGLE_BCK (M_PI / 2.0f)           // Back wheel velocity angle (90 degrees)
#define OMNI_MOTOR_ANGLE_RGT (-5.0 * M_PI / 6.0f)    // Right wheel velocity angle (-150 degrees)
#define INV_SQRT3               5.7735026918962575E-1
#define SQRT3                   1.732050807568877

#define MOT_R               0   // Right motor slot
#define MOT_B               1   // Back motor slot
#define MOT_L               2   // Left motor slot

#elif OMNI_VERSION == 20
/****************** OMNI Version 2.0*******************/
// Hardware Parameters
#define GEAR_RATIO              78.0
#define ENCODER_RES             48.0  

// MBot Omni Parameters
#define OMNI_BASE_RADIUS        0.10843     // Radius of base, from center of base to middle of omni wheels
                                            // Base radius to outer surface on wheel is 0.1227
#define OMNI_WHEEL_RADIUS       0.048       // 0.050 for old wheels
#define OMNI_MOTOR_ANGLE_LFT (-M_PI / 6.0f)   // Left wheel velocity angle (-30 degrees)
#define OMNI_MOTOR_ANGLE_BCK (M_PI / 2.0f)           // Back wheel velocity angle (90 degrees)
#define OMNI_MOTOR_ANGLE_RGT (-5.0 * M_PI / 6.0f)    // Right wheel velocity angle (-150 degrees)
#define INV_SQRT3               5.7735026918962575E-1
#define SQRT3                   1.732050807568877

#define MOT_R               0   // Right motor slot
#define MOT_B               1   // Back motor slot
#define MOT_L               2   // Left motor slot

#endif /*OMNI_VERSION*/

#endif /*MBOT_OMNI_H*/