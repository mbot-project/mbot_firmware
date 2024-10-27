#ifndef MBOT_CLASSIC_H
#define MBOT_CLASSIC_H

#include <pico/stdlib.h>
#include <pico/mutex.h>
#include <pico/time.h>
#include <hardware/gpio.h>
#include <hardware/adc.h>
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
#include "mbot_controller.h"

#include "config/mbot_classic_config.h"

/**
 * @brief Calculate the body velocity of a differential drive robot
 *
 * @param[in] wheel_left_vel  Velocity of the left wheel in rad/s
 * @param[in] wheel_right_vel Velocity of the right wheel in rad/s
 * @param[out] mbot_vel       Pointer to the structure where the calculated body velocity will be stored
 * @return int                Returns 0 on success
 */
int mbot_calculate_diff_body_vel(float wheel_left_vel, float wheel_right_vel, serial_twist2D_t *mbot_vel);

/**
 * @brief Calculate the body velocity of a differential drive robot using an IMU for angular velocity
 *
 * @param[in] wheel_left_vel  Velocity of the left wheel in rad/s
 * @param[in] wheel_right_vel Velocity of the right wheel in rad/s
 * @param[in] imu             IMU data
 * @param[out] mbot_vel       Pointer to the structure where the calculated body velocity will be stored
 * @return int                Returns 0 on success
 */
int mbot_calculate_diff_body_vel_imu(float wheel_left_vel, float wheel_right_vel, serial_mbot_imu_t imu, serial_twist2D_t *mbot_vel);

#endif /*MBOT_CLASSIC_H*/
