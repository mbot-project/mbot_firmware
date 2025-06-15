cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_PROJECT_VERSION 1.2.0 CACHE STRING "Project version")

set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/lib/pico-sdk)
include(${CMAKE_SOURCE_DIR}/lib/pico-sdk/external/pico_sdk_import.cmake)

project(mbot_pico C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DVERSION="${CMAKE_PROJECT_VERSION}")
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

# MBOT Omni
add_definitions(-DMBOT_TYPE_OMNI)
add_executable(mbot_311_firmware
  src/mbot_controller.c
  src/mbot_odometry.c
  src/mbot_print.c
  src/mbot_comms.c
  src/mbot_311_firmware.c
)
configure_mbot(mbot_311_firmware)

# Motor Calibration
set(MBOT_CALIBRATE_OMNI mbot_calibrate_omni)
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
