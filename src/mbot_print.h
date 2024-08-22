/**
 * @file mbot_print.h
 * @brief Offers functions to generate and display formatted tables in the terminal for IMU readings, encoder counts,
 * odometry, and motor velocities via a serial connection. Designed for real-time terminal updates to assist in
 * diagnostics and monitoring of the MBot's state. Typically used with Minicom for table viewing.
 */

#ifndef MBOT_PRINT_H
#define MBOT_PRINT_H

#include "mbot_comms.h"

extern uint64_t global_utime;

/**
 * @brief Generates a formatted table with integer data.
 *
 * @param[out] buf The output buffer for the table string.
 * @param[in] rows The number of rows in the table.
 * @param[in] cols The number of columns in the table.
 * @param[in] title The title of the table.
 * @param[in] headings The headings for each column.
 * @param[in] data The integer data for the table.
 */
void generateTableInt(char* buf, int rows, int cols, const char* title, const char* headings[], int data[rows][cols]);

/**
 * @brief Generates a formatted table with floating-point data.
 *
 * @param[out] buf The output buffer for the table string.
 * @param[in] rows The number of rows in the table.
 * @param[in] cols The number of columns in the table.
 * @param[in] title The title of the table.
 * @param[in] headings The headings for each column.
 * @param[in] data The floating-point data for the table.
 */
void generateTableFloat(char* buf, int rows, int cols, const char* title, const char* headings[], float data[rows][cols]);

void generateBottomLine(char* buf, int cols);

void mbot_print_state(serial_mbot_imu_t imu, serial_mbot_encoders_t encoders, serial_pose2D_t odometry, serial_mbot_motor_vel_t motor_vel);
#endif /* MBOT_PRINT_H */
