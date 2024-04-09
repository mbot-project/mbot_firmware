/**
 * This file is the main executable for the MBot OMNI firmware.
 */
#include "mbot_omni.h"

// Forward declarations and global variables
static bool running = false;
mbot_params_t params;
uint64_t timestamp_offset = 0;
uint64_t global_utime = 0;
uint64_t global_pico_time = 0;
bool global_comms_status = COMMS_ERROR; 
int drive_mode = 0;

bool mbot_loop(repeating_timer_t *rt)
{
    global_utime = to_us_since_boot(get_absolute_time()) + timestamp_offset;
    mbot_vel.utime = global_utime;
    mbot_read_encoders(&mbot_encoders);
    mbot_read_imu(&mbot_imu);
    mbot_calculate_motor_vel(mbot_encoders, &mbot_motor_vel);
    
    if(MBOT_DRIVE_TYPE == DIFFERENTIAL_DRIVE){
        mbot_calculate_diff_body_vel(   mbot_motor_vel.velocity[params.mot_left], 
                                        mbot_motor_vel.velocity[params.mot_right], 
                                        &mbot_vel
                                    );
    }
    
    else if(MBOT_DRIVE_TYPE == OMNI_120_DRIVE){
        mbot_calculate_omni_body_vel(   mbot_motor_vel.velocity[params.mot_left], 
                                        mbot_motor_vel.velocity[params.mot_right], 
                                        mbot_motor_vel.velocity[params.mot_back], 
                                        &mbot_vel
                                    );
    }

    mbot_calculate_odometry(mbot_vel, MAIN_LOOP_PERIOD, &mbot_odometry);
    mbot_odometry.utime = global_utime;
    // only run if we've got 2 way communication...
    if (global_comms_status == COMMS_OK)
    {
        if(drive_mode == MODE_MOTOR_VEL_OL){
            mbot_motor_pwm.utime = global_utime;
            mbot_motor_pwm_cmd.pwm[params.mot_right] = _calibrated_pwm_from_vel_cmd(mbot_motor_vel_cmd.velocity[params.mot_right], params.mot_right);
            mbot_motor_pwm_cmd.pwm[params.mot_left] = _calibrated_pwm_from_vel_cmd(mbot_motor_vel_cmd.velocity[params.mot_left], params.mot_left);
            if(MBOT_DRIVE_TYPE == OMNI_120_DRIVE){
                mbot_motor_pwm_cmd.pwm[params.mot_back] = _calibrated_pwm_from_vel_cmd(mbot_motor_vel_cmd.velocity[params.mot_back], params.mot_back);
            }
        }else if(drive_mode == MODE_MBOT_VEL){
            //TODO: open loop for now - implement closed loop controller
            if(MBOT_DRIVE_TYPE == OMNI_120_DRIVE){
                mbot_motor_vel_cmd.velocity[params.mot_left] = (SQRT3 / 2.0 * mbot_vel_cmd.vx - 0.5 * mbot_vel_cmd.vy - params.wheel_base_radius * mbot_vel_cmd.wz) / params.wheel_radius;
                mbot_motor_vel_cmd.velocity[params.mot_right] = (-SQRT3 / 2.0 * mbot_vel_cmd.vx - 0.5 * mbot_vel_cmd.vy - params.wheel_base_radius * mbot_vel_cmd.wz) / params.wheel_radius;
                mbot_motor_vel_cmd.velocity[params.mot_back] = (mbot_vel_cmd.vy - params.wheel_base_radius * mbot_vel_cmd.wz) / params.wheel_radius;
                float vel_left_comp = params.motor_polarity[params.mot_left] * mbot_motor_vel_cmd.velocity[params.mot_left];
                float vel_right_comp = params.motor_polarity[params.mot_right] * mbot_motor_vel_cmd.velocity[params.mot_right];
                float vel_back_comp = params.motor_polarity[params.mot_back] * mbot_motor_vel_cmd.velocity[params.mot_back];
                
                mbot_motor_pwm.utime = global_utime;
                mbot_motor_pwm_cmd.pwm[params.mot_right] = _calibrated_pwm_from_vel_cmd(vel_right_comp, params.mot_right);
                mbot_motor_pwm_cmd.pwm[params.mot_back] = _calibrated_pwm_from_vel_cmd(vel_back_comp, params.mot_back);
                mbot_motor_pwm_cmd.pwm[params.mot_left] = _calibrated_pwm_from_vel_cmd(vel_left_comp, params.mot_left);
            }else if(MBOT_DRIVE_TYPE == DIFFERENTIAL_DRIVE){
                mbot_motor_vel_cmd.velocity[params.mot_left] = (mbot_vel_cmd.vx - DIFF_BASE_RADIUS * mbot_vel_cmd.wz) / DIFF_WHEEL_RADIUS;
                mbot_motor_vel_cmd.velocity[params.mot_right] = (-mbot_vel_cmd.vx - DIFF_BASE_RADIUS * mbot_vel_cmd.wz) / DIFF_WHEEL_RADIUS;
                
                float vel_left_comp = params.motor_polarity[params.mot_left] * mbot_motor_vel_cmd.velocity[params.mot_left];
                float vel_right_comp = params.motor_polarity[params.mot_right] * mbot_motor_vel_cmd.velocity[params.mot_right];

                mbot_motor_pwm.utime = global_utime;
                mbot_motor_pwm_cmd.pwm[params.mot_right] = _calibrated_pwm_from_vel_cmd(vel_right_comp, params.mot_right);
                mbot_motor_pwm_cmd.pwm[params.mot_left] = _calibrated_pwm_from_vel_cmd(vel_left_comp, params.mot_left);
            }
        }else {
            drive_mode = MODE_MOTOR_PWM;
            mbot_motor_pwm.utime = global_utime;
        }

        // Set motors
        mbot_motor_set_duty(params.mot_right, mbot_motor_pwm_cmd.pwm[params.mot_right]);
        mbot_motor_pwm.pwm[params.mot_right] = mbot_motor_pwm_cmd.pwm[params.mot_right];
        mbot_motor_set_duty(params.mot_left, mbot_motor_pwm_cmd.pwm[params.mot_left]);
        mbot_motor_pwm.pwm[params.mot_left] = mbot_motor_pwm_cmd.pwm[params.mot_left];

        if(MBOT_DRIVE_TYPE == OMNI_120_DRIVE){
            mbot_motor_set_duty(params.mot_back, mbot_motor_pwm_cmd.pwm[params.mot_back]);
            mbot_motor_pwm.pwm[params.mot_back] = mbot_motor_pwm_cmd.pwm[params.mot_back];
        }

        // write the encoders to serial
        comms_write_topic(MBOT_ENCODERS, &mbot_encoders);
        // send odom on wire
        comms_write_topic(MBOT_ODOMETRY, &mbot_odometry);
        // write the IMU to serial
        comms_write_topic(MBOT_IMU, &mbot_imu);
        // write the Body velocity to serial
        comms_write_topic(MBOT_VEL, &mbot_vel);
        // write the Motor velocity to serial
        comms_write_topic(MBOT_MOTOR_VEL, &mbot_motor_vel);
        // write the PWMs to serial
        comms_write_topic(MBOT_MOTOR_PWM, &mbot_motor_pwm);
        //uint64_t fn_run_len = to_us_since_boot(get_absolute_time()) + timestamp_offset - cur_pico_time;
    }
    // comparing current pico time against the last successful communication timestamp(global_pico_time)
    uint64_t timeout = to_us_since_boot(get_absolute_time()) - global_pico_time;
    if(timeout > MBOT_TIMEOUT_US){
        mbot_motor_set_duty(DIFF_MOTOR_LEFT_SLOT, 0.0);
        if(MBOT_DRIVE_TYPE == OMNI_120_DRIVE){
            mbot_motor_set_duty(1, 0.0);
        }
        mbot_motor_set_duty(DIFF_MOTOR_RIGHT_SLOT, 0.0);
        global_comms_status = COMMS_ERROR;
    }

    return true;
}

int main()
{   
    running = false;
    mbot_init_pico();
    mbot_init_hardware();
    mbot_init_comms();
    mbot_read_fram(0, sizeof(params), &params);
    
    //Check also that define drive type is same as FRAM drive type
    int validate_status = validate_FRAM_data(&params);
    if (validate_status < 0)
    {
        printf("Failed to validate FRAM Data! Error code: %d\n", validate_status);
        return -1;
    }

    sleep_ms(3000);
    print_mbot_params(&params);
    printf("Starting MBot Loop...\n");
    repeating_timer_t loop_timer;
    add_repeating_timer_ms(MAIN_LOOP_PERIOD * 1000, mbot_loop, NULL, &loop_timer); // 1000x to convert to ms
    printf("Done Booting Up!\n");
    running = true;
    uint16_t counter = 0;
    
    while(running){
        // Heartbeat
        if(!(counter % 5)){
            gpio_put(LED_PIN, 1);
        }
        else if(!(counter % 7)){
            gpio_put(LED_PIN, 1);
            counter = 0;
        }
        else{
            gpio_put(LED_PIN, 0);
        }
        // Print State
        mbot_print_state(mbot_imu, mbot_encoders, mbot_odometry, mbot_motor_vel);
        sleep_ms(200);  
        counter++;
    }
}