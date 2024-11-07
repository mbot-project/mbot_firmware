cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_PROJECT_VERSION 1.1.0 CACHE STRING "Project version")

# Expose the version to the source code.
add_definitions(-DVERSION="${CMAKE_PROJECT_VERSION}")

# Add an option to toggle between CLASSIC and OMNI with CLASSIC as default
set(MBOT_TYPE "CLASSIC" CACHE STRING "MBot type to build: CLASSIC or OMNI")
set(ENC "" CACHE STRING "Encoder resolution: 20, 40, or 48")
set(OMNI_WHEEL_DIAMETER "" CACHE STRING "Wheel diameter for the Omni in mm: 101 or 96")

# Check that the user-defined option is either CLASSIC or OMNI
if(NOT MBOT_TYPE STREQUAL "CLASSIC" AND NOT MBOT_TYPE STREQUAL "OMNI")
  message(FATAL_ERROR "Invalid MBOT_TYPE. Choose either CLASSIC or OMNI.")
endif()

message("Building code for MBot type ${MBOT_TYPE}")

set(NAME_SUFFIX "_v${CMAKE_PROJECT_VERSION}")

# Validate the ENC value if it is set
if(ENC)
  if(NOT (ENC STREQUAL "20" OR ENC STREQUAL "40" OR ENC STREQUAL "48"))
    message(FATAL_ERROR "Invalid ENC value. Choose 20, 40, or 48.")
  elseif(MBOT_TYPE STREQUAL "OMNI" AND ENC STREQUAL "40")
    message(FATAL_ERROR "ENC value 40 is invalid for the Omni. Choose 20 or 48.")
  else()
    add_definitions(-DUSER_ENCODER_RES=${ENC})
    message(STATUS "Using encoder resolution: ${ENC}")
    # Update the suffix with this encoder resolution.
    set(NAME_SUFFIX "${NAME_SUFFIX}_enc${ENC}")
  endif()
endif()

# Validate the OMNI_WHEEL_DIAMETER value if it is set
if(OMNI_WHEEL_DIAMETER)
  if (MBOT_TYPE STREQUAL "CLASSIC")
    message(WARNING "Option OMNI_WHEEL_DIAMETER is only used for OMNI type. Ignoring.")
  elseif(NOT (OMNI_WHEEL_DIAMETER STREQUAL "96" OR OMNI_WHEEL_DIAMETER STREQUAL "101"))
    message(FATAL_ERROR "Invalid OMNI_WHEEL_DIAMETER value. Choose 96 or 101.")
  else()
    add_definitions(-DUSER_OMNI_WHEEL_DIAMETER=${OMNI_WHEEL_DIAMETER})
    message(STATUS "Using Omni wheel diameter: ${OMNI_WHEEL_DIAMETER}")
    # Update the suffix with this wheel diameter.
    set(NAME_SUFFIX "${NAME_SUFFIX}_w${OMNI_WHEEL_DIAMETER}mm")
  endif()
endif()

set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/lib/pico-sdk)
include(${CMAKE_SOURCE_DIR}/lib/pico-sdk/external/pico_sdk_import.cmake)

project(mbot_pico C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
  -Wno-format           # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
  -Wno-unused-function  # we have some for the docs that aren't called
  -Wno-maybe-uninitialized
)

add_subdirectory(mbot)
add_subdirectory(rc)
add_subdirectory(comms)

include_directories(
  include
  /usr/local/include
)

function(configure_mbot target)
  target_link_libraries(${target}
    pico_stdlib
    pico_multicore
    pico_time
    hardware_pwm
    hardware_sync
    hardware_i2c
    hardware_flash
    hardware_adc
    mbotlib
    rclib
    comms
  )

  pico_enable_stdio_usb(${target} 1)
  pico_enable_stdio_uart(${target} 0)
  pico_add_extra_outputs(${target})
endfunction()


if(MBOT_TYPE STREQUAL "CLASSIC")
  # MBOT Classic Main.
  add_definitions(-DMBOT_TYPE_CLASSIC)
  set(MBOT_CLASSIC mbot_classic${NAME_SUFFIX})
  add_executable(${MBOT_CLASSIC}
    src/mbot_controller.c
    src/mbot_odometry.c
    src/mbot_print.c
    src/mbot_comms.c
    src/mbot_classic.c
  )
  configure_mbot(${MBOT_CLASSIC})

  # Motor Calibration
  set(MBOT_CALIBRATE_CLASSIC mbot_calibrate_classic${NAME_SUFFIX})
  add_executable(${MBOT_CALIBRATE_CLASSIC}
    tests/mbot_calibrate_classic.c
  )
  target_link_libraries(${MBOT_CALIBRATE_CLASSIC}
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_pwm
    rclib
  )

  pico_enable_stdio_usb(${MBOT_CALIBRATE_CLASSIC} 1)
  pico_enable_stdio_uart(${MBOT_CALIBRATE_CLASSIC} 0)
  pico_add_extra_outputs(${MBOT_CALIBRATE_CLASSIC})

  # Motor Test
  add_executable(mbot_classic_motor_test
    tests/mbot_classic_motor_test.c
  )
  target_link_libraries(mbot_classic_motor_test
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_pwm
    rclib
  )
  pico_enable_stdio_usb(mbot_classic_motor_test 1)
  pico_enable_stdio_uart(mbot_classic_motor_test 0)
  pico_add_extra_outputs(mbot_classic_motor_test)

elseif(MBOT_TYPE STREQUAL "OMNI")
  # MBOT Omni
  add_definitions(-DMBOT_TYPE_OMNI)
  set(MBOT_OMNI mbot_omni${NAME_SUFFIX})
  add_executable(${MBOT_OMNI}
    src/mbot_controller.c
    src/mbot_odometry.c
    src/mbot_print.c
    src/mbot_comms.c
    src/mbot_omni.c
  )
  configure_mbot(${MBOT_OMNI})

  # Motor Calibration
  set(MBOT_CALIBRATE_OMNI mbot_calibrate_omni${NAME_SUFFIX})
  add_executable(${MBOT_CALIBRATE_OMNI}
    tests/mbot_calibrate_omni.c
  )
  target_link_libraries(${MBOT_CALIBRATE_OMNI}
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_pwm
    hardware_adc
    rclib
  )
  pico_enable_stdio_usb(${MBOT_CALIBRATE_OMNI} 1)
  pico_enable_stdio_uart(${MBOT_CALIBRATE_OMNI} 0)
  pico_add_extra_outputs(${MBOT_CALIBRATE_OMNI})

  # Motor Test
  add_executable(mbot_omni_motor_test
    tests/mbot_omni_motor_test.c
  )
  target_link_libraries(mbot_omni_motor_test
    pico_stdlib
    hardware_i2c
    hardware_pio
    hardware_pwm
    hardware_adc
    rclib
  )
  pico_enable_stdio_usb(mbot_omni_motor_test 1)
  pico_enable_stdio_uart(mbot_omni_motor_test 0)
  pico_add_extra_outputs(mbot_omni_motor_test)

endif()

# Encoder Test - Both types.
add_executable(mbot_encoder_test
  tests/mbot_encoder_test.c
)

target_link_libraries(mbot_encoder_test
  pico_stdlib
  hardware_i2c
  hardware_pio
  hardware_pwm
  rclib
)

pico_enable_stdio_usb(mbot_encoder_test 1)
pico_enable_stdio_uart(mbot_encoder_test 0)
pico_add_extra_outputs(mbot_encoder_test)

# Analog In Test
add_executable(mbot_analog_input_test
  tests/mbot_analog_input_test.c
)

target_link_libraries(mbot_analog_input_test
  pico_stdlib
  hardware_i2c
  hardware_pio
  hardware_pwm
  hardware_adc
  rclib
)

pico_enable_stdio_usb(mbot_analog_input_test 1)
pico_enable_stdio_uart(mbot_analog_input_test 0)
pico_add_extra_outputs(mbot_analog_input_test)