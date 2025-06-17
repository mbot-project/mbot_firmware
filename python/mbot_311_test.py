import time
import lcm
import threading
from mbot_lcm_msgs.mbot_motor_pwm_t import mbot_motor_pwm_t
from mbot_lcm_msgs.mbot_rob311_feedback_t import mbot_rob311_feedback_t

# Global flag to control the listening thread
listening = False

def feedback_handler(channel, data):
    """Callback function to handle received mbot_rob311_feedback_t messages"""
    msg = mbot_rob311_feedback_t.decode(data)
    print(f"latency: {int(time.time() * 1e6)-msg.utime}")

def lcm_listener(lc):
    """Function to continuously listen for LCM messages in a separate thread"""
    global listening
    while listening:
        try:
            lc.handle_timeout(100)  # 100ms timeout
        except Exception as e:
            print(f"LCM listening error: {e}")
            break

def main():
    global listening
    
    # LCM setup
    lc = lcm.LCM("udpm://239.255.76.67:7667?ttl=0")
    
    # Subscribe to feedback messages
    subscription = lc.subscribe("MBOT_ROB311_FEEDBACK", feedback_handler)
    
    # Start the LCM listener thread
    listening = True
    listener_thread = threading.Thread(target=lcm_listener, args=(lc,), daemon=True)
    listener_thread.start()
    print("Started continuous LCM listener...")

    # Parameters
    pwm = 0.5
    move_time = 6  # seconds
    command_interval = 1  # seconds between commands

    try:
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
        
        print("Motor commands completed. Listening for 5 more seconds...")
        time.sleep(5)  # Continue listening for 5 more seconds after stopping motors
        
    except KeyboardInterrupt:
        print("\nKeyboard interrupt received. Stopping motors...")
        # Emergency stop
        command = mbot_motor_pwm_t()
        command.utime = int(time.time() * 1e6)
        command.pwm[0] = 0.0
        command.pwm[1] = 0.0
        command.pwm[2] = 0.0
        lc.publish("MBOT_MOTOR_PWM_CMD", command.encode())
    
    finally:
        # Stop the listener thread
        listening = False
        print("Stopping LCM listener...")
        listener_thread.join(timeout=1)  # Wait up to 1 second for thread to finish

if __name__ == "__main__":
    main()
