#ifndef CONFIG_MBOT_OMNI_CONFIG_H
#define CONFIG_MBOT_OMNI_CONFIG_H

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
#define OMNI_WHEEL_RADIUS       ((float)USER_WHEEL_DIAMETER / 2000.0f)  // mm -> m & divide by 2
#else
#define OMNI_WHEEL_RADIUS       0.048f
#endif // USER_WHEEL_DIAMETER

// Distance from robot centre to wheel midpoint (m)
#define OMNI_BASE_RADIUS        0.10843f

#define MOT_R               0   // Right motor slot
#define MOT_B               1   // Back motor slot
#define MOT_L               2   // Left motor slot

#endif // CONFIG_MBOT_OMNI_CONFIG_H
