/**
 * @file mbot_comms.h
 * @brief Handles communication with the MBot.
 */
#ifndef MBOT_COMMS_H
#define MBOT_COMMS_H
#include <comms/common.h>
#include <comms/protocol.h>
#include <comms/listener.h>
#include <comms/topic_data.h>
#include <mbot_lcm_msgs_serial.h>
#include <pico/multicore.h>
#include <mbot/defs/mbot_params.h>
#include "mbot_channels.h"

/*
 * Messages used by the MBot code,
 * we also use these to store state
 */
// we use extern keyword here to resolve the multiple definition issue
extern serial_mbot_imu_t mbot_imu;
extern serial_pose2D_t mbot_odometry;
extern serial_mbot_encoders_t mbot_encoders;
extern serial_twist2D_t mbot_vel;
extern serial_mbot_motor_pwm_t mbot_motor_pwm;
extern serial_mbot_motor_vel_t mbot_motor_vel;
extern serial_mbot_analog_t mbot_analog_inputs;
extern serial_twist2D_t mbot_vel_cmd;
extern serial_mbot_motor_pwm_t mbot_motor_pwm_cmd;
extern serial_mbot_motor_vel_t mbot_motor_vel_cmd;
extern serial_timestamp_t mbot_received_time;

enum drive_modes
{
    MODE_MOTOR_PWM = 0,
    MODE_MOTOR_VEL_OL = 1,
    MODE_MOTOR_VEL_PID = 2,
    MODE_MBOT_VEL = 3
};

// variables are initialized in mbot_classic.c/mbot_omni.c
extern uint64_t timestamp_offset;
extern uint64_t global_pico_time;
extern bool global_comms_status;
extern int drive_mode;

// callback functions
void timestamp_cb(serial_timestamp_t *msg);
void reset_encoders_cb(serial_mbot_encoders_t *msg);
void reset_odometry_cb(serial_pose2D_t *msg);
void mbot_vel_cmd_cb(serial_twist2D_t *msg);
void mbot_motor_vel_cmd_cb(serial_mbot_motor_vel_t *msg);
void mbot_motor_pwm_cmd_cb(serial_mbot_motor_pwm_t *msg);

// others
void register_topics();
int mbot_init_comms(void);

#endif /* MBOT_COMMS_H */