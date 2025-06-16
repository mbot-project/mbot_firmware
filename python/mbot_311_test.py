import time
import lcm
from mbot_lcm_msgs.mbot_motor_pwm_t import mbot_motor_pwm_t

def main():
    # LCM setup
    lc = lcm.LCM("udpm://239.255.76.67:7667?ttl=0")

    # Parameters
    pwm = 0.5
    move_time = 6  # seconds
    command_interval = 1  # seconds between commands

    for _ in range(move_time):
        # Prepare and send command
        command = mbot_motor_pwm_t()
        cmd_utime = int(time.time() * 1e6)
        command.utime = cmd_utime
        command.pwm[0] = pwm
        command.pwm[1] = pwm
        command.pwm[2] = pwm
        lc.publish("MBOT_MOTOR_PWM_CMD", command.encode())
        time.sleep(command_interval)

    # Stop the robot
    command = mbot_motor_pwm_t()
    command.utime = int(time.time() * 1e6)
    command.pwm[0] = 0.0
    command.pwm[1] = 0.0
    command.pwm[2] = 0.0
    lc.publish("MBOT_MOTOR_PWM_CMD", command.encode())

if __name__ == "__main__":
    main()
