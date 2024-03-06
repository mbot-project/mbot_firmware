/**
 * <mbot/barometer/barometer.h>
 *
 * @brief Provides interface to the BMP280 barometer sensor on the MBot robot system using I2C.
 * @addtogroup Barometer
 * @ingroup MBot
 * @{
 */

#ifndef BAROMETER_H
#define BAROMETER_H

void mbot_initialize_barometer();
double mbot_read_barometer_pressure();
double mbot_read_barometer_temperature();

#endif