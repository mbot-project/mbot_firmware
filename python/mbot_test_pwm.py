import time
import lcm
import sys
from mbot_lcm_msgs.mbot_motor_pwm_t import mbot_motor_pwm_t

# Moves forward for 5 seconds, then stops

lc = lcm.LCM("udpm://239.255.76.67:7667?ttl=0")

# Edit these variables
pwm = 0.5
move_time = 5

command = mbot_motor_pwm_t() # A twist2D_t command encodes forward and rotational speeds of the bot
command.pwm[0] = pwm
command.pwm[1] = pwm
command.pwm[2] = pwm

lc.publish("MBOT_MOTOR_PWM_CMD",command.encode())
time.sleep(move_time)

command.pwm[0] = 0.0
command.pwm[1] = 0.0
command.pwm[2] = 0.0
lc.publish("MBOT_MOTOR_PWM_CMD",command.encode())
