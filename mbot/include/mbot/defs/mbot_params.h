#ifndef MBOT_PARAM_DEFS_H
#define MBOT_PARAM_DEFS_H

#define MBOT_ERROR -1
#define MBOT_OK 0
#define COMMS_ERROR 0
#define COMMS_OK 1
#define MBOT_TIMEOUT_US 1000000

#define SYS_CLOCK       125000 //system clock in kHz
#define PWM_FREQ        10000
#define MAIN_LOOP_HZ            25.0 // Hz of control loop
#define MAIN_LOOP_PERIOD        (1.0f / MAIN_LOOP_HZ)

#define DIFFERENTIAL_DRIVE 1
#define OMNI_120_DRIVE 2 // 3 omni wheels spaced 120deg
#define ACKERMAN_DRIVE 3

typedef struct mbot_params_t{
    int motor_polarity[3];
    int encoder_polarity[3];
    float slope_pos[3];
    float itrcpt_pos[3];
    float slope_neg[3];
    float itrcpt_neg[3];
} mbot_params_t;



#endif