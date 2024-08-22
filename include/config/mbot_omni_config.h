#ifndef CONFIG_MBOT_OMNI_CONFIG_H
#define CONFIG_MBOT_OMNI_CONFIG_H

// #if OMNI_VERSION == 10
/****************** OMNI Version 1.0*******************/
// Hardware Parameters
#define GEAR_RATIO              78.0f

/* The user can set the encoder resolution to 20 or 48 with a CMake arg. */
#ifdef USER_ENCODER_RES
#define ENCODER_RES             (float)USER_ENCODER_RES
#else
#define ENCODER_RES             48.0f  // Default encoder resolution.
#endif   /* USER_ENCODER_RES */

// MBot Omni Parameters
#define OMNI_BASE_RADIUS        0.10843f     // Radius of base, from center of base to middle of omni wheels
                                            // Base radius to outer surface on wheel is 0.1227
/* The user can set the wheel diameter to 101 or 96 mm with a CMake arg. */
#ifdef USER_OMNI_WHEEL_DIAMETER
#define OMNI_WHEEL_RADIUS       ((float)USER_OMNI_WHEEL_DIAMETER / 2000.0f)  // Convert to radius in meters.
#else
#define OMNI_WHEEL_RADIUS       0.048f       // Default wheel radius.
#endif   /* USER_ENCODER_RES */

#define MOT_R               0   // Right motor slot
#define MOT_B               1   // Back motor slot
#define MOT_L               2   // Left motor slot

#endif /* CONFIG_MBOT_OMNI_CONFIG_H */
