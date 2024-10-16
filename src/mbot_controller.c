#include "mbot_controller.h"

rc_filter_t left_wheel_pid;
rc_filter_t right_wheel_pid;
rc_filter_t back_wheel_pid;
rc_filter_t mbot_vx_pid;
rc_filter_t mbot_vy_pid;
rc_filter_t mbot_wz_pid;

int mbot_init_ctlr(mbot_ctlr_cfg_t ctlr_cfg) {
    left_wheel_pid = rc_filter_empty();
    right_wheel_pid = rc_filter_empty();
    back_wheel_pid = rc_filter_empty();
    mbot_vx_pid = rc_filter_empty();
    mbot_vy_pid = rc_filter_empty();
    mbot_wz_pid = rc_filter_empty();

    rc_filter_pid(&left_wheel_pid, ctlr_cfg.left.kp, ctlr_cfg.left.ki, ctlr_cfg.left.kd, ctlr_cfg.left.Tf, MAIN_LOOP_PERIOD);
    rc_filter_pid(&right_wheel_pid, ctlr_cfg.right.kp, ctlr_cfg.right.ki, ctlr_cfg.right.kd, ctlr_cfg.right.Tf, MAIN_LOOP_PERIOD);
    rc_filter_pid(&back_wheel_pid, ctlr_cfg.back.kp, ctlr_cfg.back.ki, ctlr_cfg.back.kd, ctlr_cfg.back.Tf, MAIN_LOOP_PERIOD);
    rc_filter_pid(&mbot_vx_pid, ctlr_cfg.vx.kp, ctlr_cfg.vx.ki, ctlr_cfg.vx.kd, ctlr_cfg.vx.Tf, MAIN_LOOP_PERIOD);
    rc_filter_pid(&mbot_vy_pid, ctlr_cfg.vy.kp, ctlr_cfg.vy.ki, ctlr_cfg.vy.kd, ctlr_cfg.vy.Tf, MAIN_LOOP_PERIOD);
    rc_filter_pid(&mbot_wz_pid, ctlr_cfg.wz.kp, ctlr_cfg.wz.ki, ctlr_cfg.wz.kd, ctlr_cfg.wz.Tf, MAIN_LOOP_PERIOD);

    return 0;
}

int mbot_motor_vel_ctlr(serial_mbot_motor_vel_t vel_cmd, serial_mbot_motor_vel_t vel, serial_mbot_motor_pwm_t *mbot_motor_pwm) {
    float right_error = vel_cmd.velocity[MOT_R] - vel.velocity[MOT_R];
    float left_error = vel_cmd.velocity[MOT_L] - vel.velocity[MOT_L];
    float right_cmd = rc_filter_march(&right_wheel_pid, right_error);
    float left_cmd = rc_filter_march(&left_wheel_pid, left_error);

    mbot_motor_pwm->pwm[MOT_R] = right_cmd;
    mbot_motor_pwm->pwm[MOT_L] = left_cmd;

    #ifdef MBOT_TYPE_OMNI
    float back_error = vel_cmd.velocity[MOT_B] - vel.velocity[MOT_B];
    float back_cmd = rc_filter_march(&back_wheel_pid, back_error);
    mbot_motor_pwm->pwm[MOT_B] = back_cmd;
    #endif

    return 0;
}

int mbot_ctlr(serial_twist2D_t vel_cmd, serial_twist2D_t vel, serial_mbot_motor_vel_t *mbot_motor_vel) {
    return 0;
}
