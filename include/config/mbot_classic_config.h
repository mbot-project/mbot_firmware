#ifndef CONFIG_MBOT_CLASSIC_CONFIG_H
#define CONFIG_MBOT_CLASSIC_CONFIG_H

// Hardware Parameters
#define GEAR_RATIO              78.0

/* The user can set the encoder resolution to 20, 40, or 48 with a CMake arg. */
#ifdef USER_ENCODER_RES
#define ENCODER_RES             (float)USER_ENCODER_RES
#else
#define ENCODER_RES             48.0  // Default encoder resolution.
#endif   /* USER_ENCODER_RES */

// MBot Classic Parameters
#define DIFF_WHEEL_DIAMETER          0.0837
#define DIFF_WHEEL_RADIUS            0.04183
#define DIFF_BASE_RADIUS             0.07786
#define MOT_R                        1   // Right motor slot
#define MOT_L                        0   // Left motor slot
#define MOT_UNUSED                   2   // Unused motor slot

#endif  /* CONFIG_MBOT_CLASSIC_CONFIG_H */
