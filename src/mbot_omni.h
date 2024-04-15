#include "mbot.h"
#include <mbot/defs/mbot_params.h>

// Hardware Parameters
#define GEAR_RATIO              78.0
#define ENCODER_RES             40.0 // 40.0 for ROB103 encoders

// MBot Omni Parameters
#define OMNI_BASE_RADIUS        0.10843     // Radius of base, from center of base to middle of omni wheels
                                            // Base radius to outer surface on wheel is 0.1227
#define OMNI_WHEEL_RADIUS       0.048       // 0.050 for old wheels
#define OMNI_MOTOR_ANGLE_LFT (-M_PI / 6.0f)   // Left wheel velocity angle (-30 degrees)
#define OMNI_MOTOR_ANGLE_BCK (M_PI / 2.0f)           // Back wheel velocity angle (90 degrees)
#define OMNI_MOTOR_ANGLE_RGT (-5.0 * M_PI / 6.0f)    // Right wheel velocity angle (-150 degrees)
#define INV_SQRT3               5.7735026918962575E-1
#define SQRT3                   1.732050807568877