#ifndef MBOT_H
#define MBOT_H

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
#include <rc/math/filter.h>
#include <rc/mpu/mpu.h>
#include <math.h>
#include <inttypes.h>

#include "mbot_comms.h"

// TODO: Decide which controller is used, open loop = 1, PID = 0
#define OPEN_LOOP 1

//Define drive type of this robot. See mbot_params.h.
// #define MBOT_DRIVE_TYPE OMNI_120_DRIVE
#define MBOT_DRIVE_TYPE DIFFERENTIAL_DRIVE

extern mbot_bhy_data_t mbot_imu_data;

// Global pointer to the i2c bus
static i2c_inst_t *i2c;

// data to hold calibration coefficients
float coeffs[12];  // 4 calibration parameters per motor 

enum drive_modes{
    MODE_MOTOR_PWM = 0,
    MODE_MOTOR_VEL_OL = 1,
    MODE_MOTOR_VEL_PID = 2,
    MODE_MBOT_VEL = 3
};


bool mbot_loop(repeating_timer_t *rt);
void mbot_read_encoders(serial_mbot_encoders_t* encoders);
void mbot_calculate_motor_vel(serial_mbot_encoders_t encoders, serial_mbot_motor_vel_t *motor_vel);

//helper functions
float _calibrated_pwm_from_vel_cmd(float vel_cmd, int motor_idx);

#endif
