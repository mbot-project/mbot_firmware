#ifndef CONFIG_MBOT_CLASSIC_CONFIG_H
#define CONFIG_MBOT_CLASSIC_CONFIG_H

#ifdef USER_GEAR_RATIO
#define GEAR_RATIO              ((float)USER_GEAR_RATIO)
#else
#define GEAR_RATIO              78.0f
#endif // USER_GEAR_RATIO

#ifdef USER_ENCODER_RES
#define ENCODER_RES             ((float)USER_ENCODER_RES)
#else
#define ENCODER_RES             48.0f
#endif // USER_ENCODER_RES

#ifdef USER_WHEEL_DIAMETER
#define DIFF_WHEEL_DIAMETER     ((float)USER_WHEEL_DIAMETER / 1000.0f)
#define DIFF_WHEEL_RADIUS       ((float)USER_WHEEL_DIAMETER / 2000.0f) // mm -> m & divide by 2
#else
#define DIFF_WHEEL_DIAMETER     0.0837f
#define DIFF_WHEEL_RADIUS       0.04185f
#endif // USER_WHEEL_DIAMETER

// Distance from robot centre to wheel midpoint (m)
#define DIFF_BASE_RADIUS             0.07786

#define MOT_R                        1   // Right motor slot
#define MOT_L                        0   // Left motor slot
#define MOT_UNUSED                   2   // Unused motor slot

#endif  // CONFIG_MBOT_CLASSIC_CONFIG_H
