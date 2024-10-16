#ifndef MBOT_CONTROLLER_H
#define MBOT_CONTROLLER_H

#include <mbot_lcm_msgs_serial.h>
#include <mbot/defs/mbot_params.h>
#include <rc/math/filter.h>

#ifdef MBOT_TYPE_CLASSIC
#include "config/mbot_classic_config.h"
#elif defined(MBOT_TYPE_OMNI)
#include "config/mbot_omni_config.h"
#endif

typedef struct mbot_pid_cfg_t{
    float kp;
    float ki;
    float kd;
    float Tf;
} mbot_pid_cfg_t;

typedef struct mbot_ctlr_cfg_t{
    mbot_pid_cfg_t right;
    mbot_pid_cfg_t left;
    mbot_pid_cfg_t back;
    mbot_pid_cfg_t vx;
    mbot_pid_cfg_t vy;
    mbot_pid_cfg_t wz;
} mbot_ctlr_cfg_t;

int mbot_init_ctlr(mbot_ctlr_cfg_t ctlr_cfg);
int mbot_motor_vel_ctlr(serial_mbot_motor_vel_t vel_cmd, serial_mbot_motor_vel_t vel, serial_mbot_motor_pwm_t *mbot_motor_pwm);
int mbot_ctlr(serial_twist2D_t vel_cmd, serial_twist2D_t vel, serial_mbot_motor_vel_t *mbot_motor_vel);

#endif // MBOT_CONTROLLER_H