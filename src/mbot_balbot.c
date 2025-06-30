/**
 * Minimal MBot v311 firmware – only publishes mbot_rob311_feedback_t.
 */
#include "mbot_balbot.h"
#include <hardware/timer.h>    // for busy_wait_us_32()
#pragma pack(1)

// Global Variables and Forward Declarations
static bool running = false;
mbot_params_t params;
uint64_t timestamp_offset = 0;
uint64_t global_utime = 0;
uint64_t global_pico_time = 0;
bool global_comms_status = COMMS_ERROR;
mbot_bhy_config_t mbot_imu_config;
mbot_bhy_data_t mbot_imu_data;
static uint64_t last_cmd_utime = 0;
// Forward declaration for internal helper function
int mbot_init_pico(void);
int mbot_init_hardware(void);

#define DEBUG_CMD_PIN 0
#define DEBUG_FB_PIN 1

/*********************************************************************
 * Main Control Functions
 * ----------------------------------------------------
 * These functions are critical for MBot's operation and include the
 * main loop and initial setup. Students may be asked to review or
 * modify parts of this section depending on their assignment.
 *********************************************************************/

bool mbot_loop(repeating_timer_t *rt)
{
    global_utime = to_us_since_boot(get_absolute_time()) + timestamp_offset;

    int64_t encoder_delta_time = global_utime - mbot_rob311_feedback.utime;
    mbot_rob311_feedback.utime = global_utime;
    mbot_rob311_feedback.enc_delta_time = (int32_t)encoder_delta_time;

    mbot_rob311_feedback.enc_ticks[MOT_L] = mbot_encoder_read_count(MOT_L);
    mbot_rob311_feedback.enc_ticks[MOT_R] = mbot_encoder_read_count(MOT_R);
    mbot_rob311_feedback.enc_ticks[MOT_B] = mbot_encoder_read_count(MOT_B);

    mbot_rob311_feedback.enc_delta_ticks[MOT_L] = mbot_encoder_read_delta(MOT_L);
    mbot_rob311_feedback.enc_delta_ticks[MOT_R] = mbot_encoder_read_delta(MOT_R);
    mbot_rob311_feedback.enc_delta_ticks[MOT_B] = mbot_encoder_read_delta(MOT_B);

    const float conversion_factor = 3.0f / (1 << 12);
    int16_t raw;
    for(int i = 0; i<4; i++){
        adc_select_input(i);
        raw = adc_read();
        mbot_rob311_feedback.volts[i] = conversion_factor * raw;
    }
    // last channel is battery voltage (has 5x divider)
    mbot_rob311_feedback.volts[3] = 5.0 * conversion_factor * raw;

    mbot_rob311_feedback.imu_angles_rpy[0] = mbot_imu_data.rpy[0];
    mbot_rob311_feedback.imu_angles_rpy[1] = mbot_imu_data.rpy[1];
    mbot_rob311_feedback.imu_angles_rpy[2] = mbot_imu_data.rpy[2];

    // only run if we've got 2 way communication...
    if (global_comms_status == COMMS_OK)
    {
        // Set motors
        mbot_motor_set_duty(MOT_R, mbot_motor_pwm_cmd.pwm[MOT_R]);
        mbot_motor_set_duty(MOT_L, mbot_motor_pwm_cmd.pwm[MOT_L]);
        mbot_motor_set_duty(MOT_B, mbot_motor_pwm_cmd.pwm[MOT_B]);

        // Print latency every time a new command is received
        if (mbot_motor_pwm_cmd.utime != last_cmd_utime) {
            printf("pi to pico one way latency: %lld\n", (long long)(to_us_since_boot(get_absolute_time()) + timestamp_offset - mbot_motor_pwm_cmd.utime));
            last_cmd_utime = mbot_motor_pwm_cmd.utime;
        }

        // write the Rob311 feedback to serial
        comms_write_topic(MBOT_ROB311_FEEDBACK, &mbot_rob311_feedback);

        // Toggle a GPIO pin to indicate that the feedback was sent
        // gpio_put(DEBUG_FB_PIN, 1);
        // busy_wait_us_32(10);
        // gpio_put(DEBUG_FB_PIN, 0);
    }
    // comparing current pico time against the last successful communication timestamp(global_pico_time)
    uint64_t timeout = to_us_since_boot(get_absolute_time()) - global_pico_time;
    if(timeout > MBOT_TIMEOUT_US){
        mbot_motor_set_duty(0, 0.0);
        mbot_motor_set_duty(1, 0.0);
        mbot_motor_set_duty(2, 0.0);
        global_comms_status = COMMS_ERROR;
    }

    return true;
}

int main()
{
    printf("********************************\n");
    printf("*  MBot Omni Firmware v%s   *\n", VERSION);
    printf("********************************\n");

    mbot_init_pico();
    mbot_init_hardware();
    mbot_init_comms();

    sleep_ms(3000);
    printf("Starting MBot Loop...\n");
    repeating_timer_t loop_timer;
    // add_repeating_timer_ms(MAIN_LOOP_PERIOD * 1000, mbot_loop, NULL, &loop_timer); // 1000x to convert to ms
    // use microsecond resolution so we can set freq to 210 hz instead of 150 -> 200 -> 250 due to millisecond resolution
    add_repeating_timer_us((int64_t)(MAIN_LOOP_PERIOD * 1e6f),
                        mbot_loop, NULL, &loop_timer);
    printf("Done Booting Up!\n");
    running = true;

    while(running){
        sleep_ms(200);
    }
}

/******************************************************
 * Helper Functions
 * ----------------------------------------------------
 * These functions are used internally by the main control functions.
 * They are not intended for modification by students. These functions
 * provide lower-level control and utility support.
 ******************************************************/
int mbot_init_pico(void){
    bi_decl(bi_program_description("Firmware for the MBot Robot Control Board"));

    // set master clock to 250MHz (if unstable set SYS_CLOCK to 125Mhz)
    if(!set_sys_clock_khz(125000, true)){
        printf("ERROR mbot_init_pico: cannot set system clock\n");
        return MBOT_ERROR;
    };

    stdio_init_all(); // enable USB serial terminal
    sleep_ms(500);
    printf("\nMBot Booting Up!\n");
    return MBOT_OK;
}

int mbot_init_hardware(void){
    sleep_ms(1000);
    // Initialize Motors
    printf("initializinging motors...\n");
    mbot_motor_init(0);
    mbot_motor_init(1);
    mbot_motor_init(2);
    printf("initializinging encoders...\n");
    mbot_encoder_init();

    // Initialize the IMU
    mbot_imu_config = mbot_imu_default_config();
    mbot_imu_config.sample_rate = 200;
    printf("Initializing IMU...\n");
    mbot_imu_init(&mbot_imu_data, mbot_imu_config);

    // Initialize ADC
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_gpio_init(29);

    // Initialize debug GPIO pins for latency measurement
    // gpio_init(DEBUG_CMD_PIN);
    // gpio_set_dir(DEBUG_CMD_PIN, GPIO_OUT);
    // gpio_put(DEBUG_CMD_PIN, 0);   // ensure low at start

    // gpio_init(DEBUG_FB_PIN);
    // gpio_set_dir(DEBUG_FB_PIN, GPIO_OUT);
    // gpio_put(DEBUG_FB_PIN, 0);

    return MBOT_OK;
}