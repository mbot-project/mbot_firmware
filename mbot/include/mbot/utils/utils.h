#ifndef MBOT_UTILS_H
#define MBOT_UTILS_H
#include <mbot/defs/mbot_params.h>

int mbot_init_i2c();
int _mbot_init_i2c(unsigned int pico_sda_pin, unsigned int pico_scl_pin);
void _i2c_bus_recover(unsigned int sda_pin, unsigned int scl_pin);
int _check_i2c0_enabled();
int validate_mbot_classic_FRAM_data(mbot_params_t* params, int mot_left, int mot_right, int mot_unused);
int validate_mbot_omni_FRAM_data(mbot_params_t* params, int mot_left, int mot_right, int mot_back);

#endif