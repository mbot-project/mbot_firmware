#include <mbot_lcm_msgs_serial.h>
#include <mbot/defs/mbot_params.h>
#include <math.h>

/**
 * @brief Calculate the odometry (position and orientation) of the robot based on its body velocity
 *
 * @param[in] mbot_vel  Current body velocity of the robot
 * @param[in] dt        Time interval over which the velocity is applied
 * @param[out] odometry Pointer to the structure where the calculated odometry will be stored
 * @return int          Returns 0 on success
 */
int mbot_calculate_odometry(serial_twist2D_t mbot_vel,  float dt, serial_pose2D_t *odometry);
