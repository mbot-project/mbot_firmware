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
#include <mbot/utils/utils.h>
#include <rc/math/filter.h>
#include <rc/mpu/mpu.h>
#include <math.h>
#include <inttypes.h>

#include "mbot_comms.h"

// TODO: Decide which controller is used, open loop = 1, PID = 0
#define OPEN_LOOP 1

// initializing in mbot.c
extern mbot_bhy_data_t mbot_imu_data;

// initializing in mbot_classic.c or 
extern mbot_params_t params;

// Global pointer to the i2c bus
static i2c_inst_t *i2c;


//shared functions for omni and classic
int mbot_init_pico(void);
int mbot_init_hardware(void);
void print_mbot_params(const mbot_params_t* params);

bool mbot_loop(repeating_timer_t *rt);
void mbot_read_encoders(serial_mbot_encoders_t* encoders);
void mbot_calculate_motor_vel(serial_mbot_encoders_t encoders, serial_mbot_motor_vel_t *motor_vel);

//helper functions
float _calibrated_pwm_from_vel_cmd(float vel_cmd, int motor_idx);

#endif /* MBOT_H */
