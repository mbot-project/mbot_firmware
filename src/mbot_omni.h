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

#include "config/mbot_omni_config.h"

// Some useful math definitions.
#define OMNI_MOTOR_ANGLE_LFT (-M_PI / 6.0f)   // Left wheel velocity angle (-30 degrees)
#define OMNI_MOTOR_ANGLE_BCK (M_PI / 2.0f)           // Back wheel velocity angle (90 degrees)
#define OMNI_MOTOR_ANGLE_RGT (-5.0f * M_PI / 6.0f)    // Right wheel velocity angle (-150 degrees)
#define SQRT3                   1.732050807568877f
#define INV_SQRT3               5.7735026918962575E-1f

/**
 * @brief Calculate the body velocity of an omnidirectional (Kiwi) robot
 *
 * @param[in] wheel_left_vel  Velocity of left wheel in rad/s
 * @param[in] wheel_right_vel Velocity of right wheelin rad/s
 * @param[in] wheel_back_vel  Velocity of back wheel in rad/s
 * @param[out] mbot_vel   Pointer to the structure where the calculated body velocity will be stored
 * @return int            Returns 0 on success
 */
int mbot_calculate_omni_body_vel(float wheel_left_vel, float wheel_right_vel, float wheel_back_vel, serial_twist2D_t *mbot_vel);

/**
 * @brief Calculate the body velocity of an omnidirectional (Kiwi) robot using an IMU for angular velocity
 *
 * @param[in] wheel_left_vel  Velocity of left wheel in rad/s
 * @param[in] wheel_right_vel Velocity of right wheelin rad/s
 * @param[in] wheel_back_vel  Velocity of back wheel in rad/s
 * @param[in] imu         IMU data
 * @param[out] mbot_vel   Pointer to the structure where the calculated body velocity will be stored
 * @return int            Returns 0 on success
 */
int mbot_calculate_omni_body_vel_imu(float wheel_left_vel, float wheel_right_vel, float wheel_back_vel, serial_mbot_imu_t imu, serial_twist2D_t *mbot_vel);

#endif /*MBOT_OMNI_H*/
