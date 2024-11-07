#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "mbot_print.h"

#pragma pack(1)

void generateTableInt(char* buf, int rows, int cols, const char* title, const char* headings[], int data[rows][cols]) {
    char line[256] = {0};
    int title_len = strlen(title);
    int line_len = cols * 12;  // Each column is 12 chars wide

    // Top line
    memset(line, '-', line_len-1);
    line[line_len] = '\0';
    sprintf(buf, "|%s|\n", line);

    // Title (yellow color)
    sprintf(buf + strlen(buf), "|\033[33m %s%*s\033[0m|\n", title, line_len - title_len - 2, "");

    // Headings (blue color)
    for (int i = 0; i < cols; i++) {
        sprintf(buf + strlen(buf), "|\033[34m  %s%*s\033[0m", headings[i], 9 - (int)strlen(headings[i]), "");
    }
    sprintf(buf + strlen(buf), "|\n");

    // Divider
    for (int i = 0; i < cols; i++) {
        sprintf(buf + strlen(buf), "|%s", "-----------");
    }
    sprintf(buf + strlen(buf), "|\n");

    // Data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sprintf(buf + strlen(buf), "| %9d ", data[i][j]);
        }
        sprintf(buf + strlen(buf), "|\n");
    }

    // Bottom line
    //sprintf(buf + strlen(buf), "|%s|\n", line);
}

void generateTableFloat(char* buf, int rows, int cols, const char* title, const char* headings[], float data[rows][cols]) {
    char line[256] = {0};
    int title_len = strlen(title);
    int line_len = cols * 12;  // Each column is 12 chars wide

    // Top line
    memset(line, '-', line_len-1);
    line[line_len] = '\0';
    sprintf(buf, "|%s|\n", line);

    // Title (yellow color)
    sprintf(buf + strlen(buf), "|\033[33m %s%*s\033[0m|\n", title, line_len - title_len - 2, "");

    // Headings (blue color)
    for (int i = 0; i < cols; i++) {
        sprintf(buf + strlen(buf), "|\033[34m  %s%*s\033[0m", headings[i], 9 - (int)strlen(headings[i]), "");
    }
    sprintf(buf + strlen(buf), "|\n");

    // Divider
    for (int i = 0; i < cols; i++) {
        sprintf(buf + strlen(buf), "|%s", "-----------");
    }
    sprintf(buf + strlen(buf), "|\n");

    // Data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            sprintf(buf + strlen(buf), "|  %8.4f ", data[i][j]);
        }
        sprintf(buf + strlen(buf), "|\n");
    }

    // Bottom line
    //sprintf(buf + strlen(buf), "|%s|\n", line);
}

void generateBottomLine(char* buf, int cols) {
    char line[256] = {0};
    int line_len = cols * 12;  // Each column is 12 chars wide
    memset(line, '-', line_len-1);
    line[line_len] = '\0';
    sprintf(buf, "|%s|\n", line);
}

void mbot_print_state(serial_mbot_imu_t imu, serial_mbot_encoders_t encoders, serial_pose2D_t odometry, serial_mbot_motor_vel_t motor_vel){
    printf("\033[2J\r");
    if(global_comms_status == COMMS_OK){
        printf("| \033[32m COMMS OK \033[0m TIME: %lld |\n", global_utime);
    }
    else{
        printf("| \033[31m SERIAL COMMUNICATION FAILURE\033[0m     |\n");
    }
    const char* imu_headings[] = {"ROLL", "PITCH", "YAW"};
    const char* analog_headings[] = {"AIN 0","AIN 1","AIN 2","BATT (V)"};
    const char* enc_headings[] = {"ENC 0", "ENC 1", "ENC 2"};
    const char* odom_headings[] = {"X", "Y", "THETA"};
    const char* motor_vel_headings[] = {"MOT 0", "MOT 1", "MOT 2"};
    char buf[1024] = {0};

    float adc_array[4] = {mbot_analog_inputs.volts[0], mbot_analog_inputs.volts[1], mbot_analog_inputs.volts[2], mbot_analog_inputs.volts[3]};
    generateTableFloat(buf, 1, 4, "ANALOG", analog_headings, adc_array);
    printf("\r%s", buf);
    buf[0] = '\0';
    // we shouldn't need to do this, need to update generateTable to handle different datatypes
    int encs[3] = {(int)encoders.ticks[0], (int)encoders.ticks[1], (int)encoders.ticks[2]};
    generateTableInt(buf, 1, 3, "ENCODERS", enc_headings, encs);
    printf("\r%s", buf);
    
    buf[0] = '\0';
    generateTableFloat(buf, 1, 3, "IMU", imu_headings, imu.angles_rpy);
    printf("\r%s", buf);
    
    buf[0] = '\0';
    generateTableFloat(buf, 1, 3, "MOTOR", motor_vel_headings, motor_vel.velocity);
    printf("\r%s", buf);
    
    buf[0] = '\0';
    float odom_array[3] = {odometry.x, odometry.y, odometry.theta};
    generateTableFloat(buf, 1, 3, "ODOMETRY", odom_headings, odom_array);
    printf("\r%s", buf);

    buf[0] = '\0';
    generateBottomLine(buf, 3);
    printf("\r%s\n", buf);
}