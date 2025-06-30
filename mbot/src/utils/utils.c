#include <mbot/utils/utils.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <mbot/defs/mbot_params.h>

//Always operates on the I2C0 line.

//Checks I2C enable status and initialize iff I2C hasn't been enabled already
//Returns 1 if i2c was initialized as a result, 0 if I2C was already enabled
int mbot_init_i2c(){
    return _mbot_init_i2c(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN);
}

int _mbot_init_i2c(unsigned int pico_sda_pin, unsigned int pico_scl_pin)
{
    if(!_check_i2c0_enabled()){
        _i2c_bus_recover(pico_sda_pin, pico_scl_pin);
        i2c_init(i2c0, 400 * 1000);
        gpio_set_function(pico_sda_pin, GPIO_FUNC_I2C);
        gpio_set_function(pico_scl_pin, GPIO_FUNC_I2C);
        gpio_pull_up(pico_sda_pin);
        gpio_pull_up(pico_scl_pin);
        return 1;
    }
    return 0;
}

int _check_i2c0_enabled(){
    // 0x9C: I2C_ENABLE_STATUS has bit 0 = 1 when initialized and 0 when not (default 0)
    return *(volatile uint32_t*)(I2C0_BASE + 0x9C) & 1;
}

void _i2c_bus_recover(unsigned int sda_pin, unsigned int scl_pin)
{
    // Temporarily switch pins to GPIO w/ pull-ups
    gpio_set_function(sda_pin, GPIO_FUNC_SIO);
    gpio_set_function(scl_pin, GPIO_FUNC_SIO);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    gpio_set_dir(sda_pin, GPIO_IN);   // to listen to pin state
    gpio_set_dir(scl_pin, GPIO_OUT);  // to control the pin state
    gpio_put(scl_pin, 1);             // make sure SCL is high

    // If SCL is being held low we cannot recover in software
    if(!gpio_get(scl_pin)){
        return;
    }

    // Clock out up to 16 pulses while SDA stays low
    for(int i = 0; i < 16 && !gpio_get(sda_pin); i++){
        gpio_put(scl_pin, 0);
        sleep_us(5);
        gpio_put(scl_pin, 1);
        sleep_us(5);
    }

    // Generate a STOP condition (SDA low -> SCL high -> SDA high)
    gpio_set_dir(sda_pin, GPIO_OUT);
    gpio_put(sda_pin, 0);
    sleep_us(5);
    gpio_put(scl_pin, 1);
    sleep_us(5);
    gpio_set_dir(sda_pin, GPIO_IN);   // release SDA
    sleep_us(5);
}

//Validates mbot classic calibration in FRAM.
int validate_mbot_classic_FRAM_data(mbot_params_t* params, int mot_left, int mot_right, int mot_unused){
    for(int idx = 0; idx < 3; ++idx){
        if(idx == mot_unused){
            continue; //Don't look for slope/intercept on back wheel that doesn't exist
        }
        if(params->motor_polarity[idx] != 1 && params->motor_polarity[idx] != -1){
            //Invalid motor polarity
            return -1;
        }
        if(params->encoder_polarity[idx] != 1 && params->encoder_polarity[idx] != -1){
            //Invalid encoder polarity
            return -2;
        }
    }
    if(mot_left > 3 || mot_left < 0){
        //Invalid left motor pin
        return -3;
    }
    if(mot_right > 3 || mot_right < 0){
        //Invalid right motor pin
        return -4;
    }

    for(int idx = 0; idx < 3; ++idx){
        if(idx == mot_unused){
            continue; //Don't look for slope/intercept on back wheel that doesn't exist
        }
        if(params->slope_pos[idx] <= 0 || params->itrcpt_pos[idx] < 0 || params->slope_neg[idx] <= 0 || params->itrcpt_neg[idx] > 0){
            //Invalid slope/intercept
            return -6;
        }
    }

    return 0;
}

//Validates mbot omni calibration in FRAM.
int validate_mbot_omni_FRAM_data(mbot_params_t* params, int mot_left, int mot_right, int mot_back){
    for(int idx = 0; idx < 3; ++idx){
        if(params->motor_polarity[idx] != 1 && params->motor_polarity[idx] != -1){
            //Invalid motor polarity
            return -1;
        }
        if(params->encoder_polarity[idx] != 1 && params->encoder_polarity[idx] != -1){
            //Invalid encoder polarity
            return -2;
        }
    }
    if(mot_left > 3 || mot_left < 0){
        //Invalid left motor pin
        return -3;
    }
    if(mot_right > 3 || mot_right < 0){
        //Invalid right motor pin
        return -4;
    }
    if(mot_back > 3 || mot_back < 0){
        //Invalid back motor pin
        return -5;
    }

    for(int idx = 0; idx < 3; ++idx){
        if(params->slope_pos[idx] <= 0 || params->itrcpt_pos[idx] < 0 || params->slope_neg[idx] <= 0 || params->itrcpt_neg[idx] > 0){
            //Invalid slope/intercept
            return -6;
        }
    }

    return 0;
}