#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <pico/stdlib.h>
#include <pico/binary_info.h>
#include <mbot/imu/imu.h>
#include <mbot/motor/motor.h>
#include <mbot/encoder/encoder.h>
#include <mbot/fram/fram.h>
#include <mbot/defs/mbot_params.h>

#include "config/mbot_classic_config.h"

mbot_bhy_data_t mbot_imu_data;
mbot_bhy_config_t mbot_imu_config;

void find_two_smallest(float* arr, int size, int* idx1, int* idx2) {
    *idx1 = 0;
    *idx2 = 1;

    if (fabs(arr[*idx2]) < fabs(arr[*idx1])) {
        // swap
        int temp = *idx1;
        *idx1 = *idx2;
        *idx2 = temp;
    }

    for (int i = 2; i < size; i++) {
        if (fabs(arr[i]) < fabs(arr[*idx1])) {
            *idx2 = *idx1;
            *idx1 = i;
        } else if (fabs(arr[i]) < fabs(arr[*idx2])) {
            *idx2 = i;
        }
    }
}

int find_index_of_max_positive(float* arr, int size) {
    float max_positive = -5000.0;
    int idx = -1;
    for (int i = 0; i < size; i++) {
        if (arr[i] > max_positive && arr[i] > 0) {
            max_positive = arr[i];
            idx = i;
        }
    }
    // Return -1 if no positive number is found
    return idx;
}

void least_squares_fit(float* pwms, float* speeds, int n, float* m, float* b) {
    float sum_x = 0.0, sum_y = 0.0, sum_xy = 0.0, sum_xx = 0.0;

    for (int i = 0; i < n; ++i) {
        if (speeds[i] != 0.0) {
            sum_x += speeds[i];
            sum_y += pwms[i];
            sum_xy += speeds[i] * pwms[i];
            sum_xx += speeds[i] * speeds[i];
        }
    }

    *m = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    *b = (sum_y - *m * sum_x) / n;
}

void print_mbot_params_dd(const mbot_params_t* params) {
    printf("Motor Polarity: %d %d\n", params->motor_polarity[MOT_L], params->motor_polarity[MOT_R]);
    printf("Encoder Polarity: %d %d\n", params->encoder_polarity[MOT_L], params->encoder_polarity[MOT_R]);

    printf("Positive Slope: %f %f\n", params->slope_pos[MOT_L], params->slope_pos[MOT_R]);
    printf("Positive Intercept: %f %f\n", params->itrcpt_pos[MOT_L], params->itrcpt_pos[MOT_R]);

    printf("Negative Slope: %f %f\n", params->slope_neg[MOT_L], params->slope_neg[MOT_R]);
    printf("Negative Intercept: %f %f\n", params->itrcpt_neg[MOT_L], params->itrcpt_neg[MOT_R]);
}

int main() {
    // Initialization
    mbot_params_t params;
    stdio_init_all();
    printf("\n\n\nInitializing...\n");
    bi_decl(bi_program_description("This will calibrate an MBot and print a diagnostic report"));
    mbot_motor_init(MOT_L);
    mbot_motor_init(MOT_R);
    mbot_encoder_init();
    mbot_init_fram();
    printf("\nWaiting for 5 seconds...\n");
    sleep_ms(5000);

    // Find Encoder Polarity
    printf("\nTesting Encoder Polarity...\n");
    mbot_motor_set_duty(MOT_L, 0.2);
    mbot_motor_set_duty(MOT_R, 0.2);
    for(int i=0; i<5; i++){
        printf("E0: %d , E1: %d\n", mbot_encoder_read_delta(MOT_L), mbot_encoder_read_delta(MOT_R));
        sleep_ms(100);
    }
    mbot_motor_set_duty(MOT_L, 0.0);
    mbot_motor_set_duty(MOT_R, 0.0);
    params.encoder_polarity[MOT_L] = (mbot_encoder_read_count(MOT_L)>0) ? 1 : -1;
    params.encoder_polarity[MOT_R] = (mbot_encoder_read_count(MOT_R)>0) ? 1 : -1;
    printf("\nENC0 POL: %d , ENC1 POL: %d\n \n", params.encoder_polarity[MOT_L], params.encoder_polarity[MOT_R]);
    
    // Find Motor Polarity
    mbot_imu_config = mbot_imu_default_config();
    mbot_imu_init(&mbot_imu_data, mbot_imu_config);
    printf("\nTesting Motor Polarity...\n");

    // initializing array to store imu data
    float gyro_z[4] = {0, 0, 0, 0};

    float spd = 0.4; //Motor duty used for calibration
    float motor_duties[4][2] = {
        {spd, spd},
        {-spd, spd},
        {spd, -spd},
        {-spd, -spd}
    };

    for (int i = 0; i < 4; i++) {
        mbot_motor_set_duty(MOT_L, motor_duties[i][0]);
        mbot_motor_set_duty(MOT_R, motor_duties[i][1]);

        for (int j = 0; j < 25; j++) {
            gyro_z[i] += mbot_imu_data.gyro[2];
            sleep_ms(20);
        }

        mbot_motor_set_duty(MOT_L, 0.0);
        mbot_motor_set_duty(MOT_R, 0.0);
        printf("Gyro: %f\n", gyro_z[i]);
        sleep_ms(500);
    }

    int rot_idx;
    rot_idx = find_index_of_max_positive(gyro_z, 4); //Find largest positive rotation (+wz)
    printf("rotidx = %d\n", rot_idx);
    
    // motor_duties[rot_idx] will make the robot turnning CCW
    switch(rot_idx) {
    case 0:
        params.motor_polarity[MOT_L] = -1;
        params.motor_polarity[MOT_R] = -1;
        break;
    case 1:
        params.motor_polarity[MOT_L] = 1;
        params.motor_polarity[MOT_R] = -1;
        break;
    case 2:
        params.motor_polarity[MOT_L] = -1;
        params.motor_polarity[MOT_R] = 1;
        break;
    case 3:
        params.motor_polarity[MOT_L] = 1;
        params.motor_polarity[MOT_R] = 1;
        break;
    default:
        printf("ERROR: Invalid index\n");
    }
    
    // Adjust Encoder Polarity
    params.encoder_polarity[MOT_L] *= params.motor_polarity[MOT_L];
    params.encoder_polarity[MOT_R] *= params.motor_polarity[MOT_R];

    printf("Motor Polarity: (%d, %d)  Left ID: %d, Right ID: %d\n", params.motor_polarity[MOT_L], params.motor_polarity[MOT_R], MOT_L, MOT_R);

    printf("\nTesting polarity calibration result: \n");

    int enc_right;
    int enc_left;

    printf("Driving Forward...\n");
    mbot_encoder_read_delta(MOT_R);
    mbot_encoder_read_delta(MOT_L);
    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R]*-0.2);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L]*0.2);
    sleep_ms(500);
    mbot_motor_set_duty(MOT_R, 0.0);
    mbot_motor_set_duty(MOT_L, 0.0);
    enc_right = params.encoder_polarity[MOT_R] * mbot_encoder_read_delta(MOT_R);
    enc_left = params.encoder_polarity[MOT_L] * mbot_encoder_read_delta(MOT_L);
    printf("Encoder Readings: R: %d, L: %d, \n", enc_right, enc_left);
    sleep_ms(500);

    printf("Driving Backward...\n");
    mbot_encoder_read_delta(MOT_R);
    mbot_encoder_read_delta(MOT_L);
    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R]*0.2);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L]*-0.2);
    sleep_ms(500);
    mbot_motor_set_duty(MOT_R, 0.0);
    mbot_motor_set_duty(MOT_L, 0.0);
    enc_right = params.encoder_polarity[MOT_R] * mbot_encoder_read_delta(MOT_R);
    enc_left = params.encoder_polarity[MOT_L] * mbot_encoder_read_delta(MOT_L);
    printf("Encoder Readings: R: %d, L: %d, \n", enc_right, enc_left);
    sleep_ms(500);

    printf("Turning Positive (CCW)...\n");
    mbot_encoder_read_delta(MOT_R);
    mbot_encoder_read_delta(MOT_L);
    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R]*-0.2);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L]*-0.2);
    sleep_ms(500);
    mbot_motor_set_duty(MOT_R, 0.0);
    mbot_motor_set_duty(MOT_L, 0.0);
    enc_right = params.encoder_polarity[MOT_R] * mbot_encoder_read_delta(MOT_R);
    enc_left = params.encoder_polarity[MOT_L] * mbot_encoder_read_delta(MOT_L);
    printf("Encoder Readings: R: %d, L: %d, \n", enc_right, enc_left);
    sleep_ms(500);

    printf("Turning Negative (CW)...\n");
    mbot_encoder_read_delta(MOT_R);
    mbot_encoder_read_delta(MOT_L);
    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R]*0.2);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L]*0.2);
    sleep_ms(500);
    mbot_motor_set_duty(MOT_R, 0.0);
    mbot_motor_set_duty(MOT_L, 0.0);
    enc_right = params.encoder_polarity[MOT_R] * mbot_encoder_read_delta(MOT_R);
    enc_left = params.encoder_polarity[MOT_L] * mbot_encoder_read_delta(MOT_L);
    printf("Encoder Readings: R: %d, L: %d, \n", enc_right, enc_left);
    sleep_ms(500);

    // Find slope and intercept
    printf("\nCalculating Slope and Intercept...\n");
    
    // Turn CCW
    int num_points = 20;
    float dt = 0.5;
    float wheel_speed_right[num_points+1];
    float wheel_speed_left[num_points+1];
    float duty_right[num_points+1];
    float duty_left[num_points+1];
    float conv = (2 * M_PI)/(GEAR_RATIO * ENCODER_RES);
    printf("Measuring CCW...\n");
    mbot_encoder_read_delta(MOT_R);
    mbot_encoder_read_delta(MOT_L);
    
    for(int i = 0; i <= num_points; i++){
        
        float d = i * 1.0/(float)num_points;
        mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R] * -d);
        mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L] * -d);
        sleep_ms(dt * 1000);
        duty_right[i] = -d;
        duty_left[i] = -d;
        wheel_speed_right[i] = conv * params.encoder_polarity[MOT_R] * mbot_encoder_read_delta(MOT_R) / dt;
        wheel_speed_left[i] = conv * params.encoder_polarity[MOT_L] * mbot_encoder_read_delta(MOT_L) / dt;
        printf("duty: %f, right: %f, left: %f\n", duty_right[i], wheel_speed_right[i], wheel_speed_left[i]);
    }
    
    int n = sizeof(duty_right) / sizeof(duty_right[0]);
    float m_rn, b_rn, m_ln, b_ln;
    least_squares_fit(duty_right, wheel_speed_right, n, &m_rn, &b_rn);
    least_squares_fit(duty_left, wheel_speed_left, n, &m_ln, &b_ln);
    
    //slow down

    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R] * -0.8);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L] * -0.8);
    sleep_ms(300);
    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R] * -0.5);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L] * -0.5);
    sleep_ms(300);
    mbot_motor_set_duty(MOT_R, 0.0);
    mbot_motor_set_duty(MOT_L, 0.0);
    printf("\n\n");
    

    //Turn CW
    sleep_ms(500);
    printf("Measuring CW...\n");
    mbot_encoder_read_delta(MOT_R);
    mbot_encoder_read_delta(MOT_L);
    for(int i = 0; i <= num_points; i++){
        float d = i * 1.0/(float)num_points;
        mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R] * d);
        mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L] * d);
        sleep_ms(dt * 1000);
        duty_right[i] = d;
        duty_left[i] = d;
        wheel_speed_right[i] = conv * params.encoder_polarity[MOT_R] * mbot_encoder_read_delta(MOT_R) / dt;
        wheel_speed_left[i] = conv * params.encoder_polarity[MOT_L] * mbot_encoder_read_delta(MOT_L) / dt;
        printf("duty: %f, right: %f, left: %f\n", duty_right[i], wheel_speed_right[i], wheel_speed_left[i]);
    }

    //slow down
    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R] * 0.8);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L] * 0.8);
    sleep_ms(300);
    mbot_motor_set_duty(MOT_R, params.motor_polarity[MOT_R] * 0.5);
    mbot_motor_set_duty(MOT_L, params.motor_polarity[MOT_L] * 0.5);
    sleep_ms(300);
    mbot_motor_set_duty(MOT_R, 0.0);
    mbot_motor_set_duty(MOT_L, 0.0);

    float m_rp, b_rp, m_lp, b_lp;
    least_squares_fit(duty_right, wheel_speed_right, n, &m_rp, &b_rp);
    least_squares_fit(duty_left, wheel_speed_left, n, &m_lp, &b_lp);
    
    params.slope_pos[MOT_R] = m_rp;
    params.slope_pos[MOT_L] = m_lp;
    params.slope_neg[MOT_R] = m_rn;
    params.slope_neg[MOT_L] = m_ln;
    params.itrcpt_pos[MOT_R] = b_rp;
    params.itrcpt_pos[MOT_L] = b_lp;
    params.itrcpt_neg[MOT_R] = b_rn;
    params.itrcpt_neg[MOT_L] = b_ln;

    printf("Right Motor Calibration: \n");
    printf("m_rp: %f\n", m_rp);
    printf("b_rp: %f\n", b_rp);
    printf("m_rn: %f\n", m_rn);
    printf("b_rn: %f\n", b_rn);
    printf("Left Motor Calibration: \n");
    printf("m_lp: %f\n", m_lp);
    printf("b_lp: %f\n", b_lp);
    printf("m_ln: %f\n", m_ln);
    printf("b_ln: %f\n", b_ln);

    
    mbot_write_fram(0, sizeof(params), &params);
    mbot_params_t written;
    mbot_read_fram(0, sizeof(written), &written);
    printf("\nParameters stored in FRAM (%d bytes): \n", sizeof(written));
    print_mbot_params_dd(&written);

    printf("\nDone!\n");
    fflush(stdout);  // Make sure all output is sent to the terminal
    sleep_ms(1000);   
}