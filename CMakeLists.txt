cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_PROJECT_VERSION 1.2.0 CACHE STRING "Project version")

# Expose the version to the source code.
add_definitions(-DVERSION="${CMAKE_PROJECT_VERSION}")

# Add an option to toggle between CLASSIC, OMNI, or BALBOT
set(MBOT_TYPE "" CACHE STRING "MBot type to build (optional): CLASSIC, OMNI, or BALBOT")
set(ENC "" CACHE STRING "Encoder resolution (optional)")
set(WHEEL_DIAMETER "" CACHE STRING "Wheel diameter in mm (optional)")
set(GEAR_RATIO "" CACHE STRING "Gear ratio (optional)")

# Check that the user-defined option is either CLASSIC, OMNI, or BALBOT
if(MBOT_TYPE AND NOT (MBOT_TYPE STREQUAL "CLASSIC" OR MBOT_TYPE STREQUAL "OMNI" OR MBOT_TYPE STREQUAL "BALBOT"))
  message(FATAL_ERROR "Invalid MBOT_TYPE. Choose CLASSIC, OMNI, or BALBOT.")
endif()

if(MBOT_TYPE)
  message("Building code for MBot type ${MBOT_TYPE}")
else()
  message("Building code for ALL MBot types")
endif()

set(NAME_SUFFIX "_v${CMAKE_PROJECT_VERSION}")

if(ENC)
  add_definitions(-DUSER_ENCODER_RES=${ENC})
  message(STATUS "Using encoder resolution: ${ENC}")
  set(NAME_SUFFIX "${NAME_SUFFIX}_enc${ENC}")
endif()

if(WHEEL_DIAMETER)
  add_definitions(-DUSER_WHEEL_DIAMETER=${WHEEL_DIAMETER})
  message(STATUS "Using wheel diameter: ${WHEEL_DIAMETER}")
  set(NAME_SUFFIX "${NAME_SUFFIX}_w${WHEEL_DIAMETER}mm")
endif()

if(GEAR_RATIO)
  add_definitions(-DUSER_GEAR_RATIO=${GEAR_RATIO})
  message(STATUS "Using gear ratio: ${GEAR_RATIO}")
  set(NAME_SUFFIX "${NAME_SUFFIX}_gr${GEAR_RATIO}")
endif()

set(PICO_SDK_PATH ${CMAKE_SOURCE_DIR}/lib/pico-sdk)
include(${CMAKE_SOURCE_DIR}/lib/pico-sdk/external/pico_sdk_import.cmake)

project(mbot_pico C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Override the default extra outputs helper, only creates UF2 files.
# Comment this out to create all the files.
macro(pico_add_extra_outputs target)
  pico_add_uf2_output(${target})
endmacro()

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

if(NOT MBOT_TYPE OR MBOT_TYPE STREQUAL "CLASSIC")
  # MBOT Classic Main.
  set(MBOT_CLASSIC mbot_classic${NAME_SUFFIX})
  add_executable(${MBOT_CLASSIC}
    src/mbot_controller.c
    src/mbot_odometry.c
    src/mbot_print.c
    src/mbot_comms.c
    src/mbot_classic.c
  )
  configure_mbot(${MBOT_CLASSIC})
  target_compile_definitions(${MBOT_CLASSIC} PRIVATE MBOT_TYPE_CLASSIC)

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
  target_compile_definitions(${MBOT_CALIBRATE_CLASSIC} PRIVATE MBOT_TYPE_CLASSIC)

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
  target_compile_definitions(mbot_classic_motor_test PRIVATE MBOT_TYPE_CLASSIC)
  pico_enable_stdio_usb(mbot_classic_motor_test 1)
  pico_enable_stdio_uart(mbot_classic_motor_test 0)
  pico_add_extra_outputs(mbot_classic_motor_test)
endif()

if(NOT MBOT_TYPE OR MBOT_TYPE STREQUAL "OMNI")
  # MBOT Omni
  set(MBOT_OMNI mbot_omni${NAME_SUFFIX})
  add_executable(${MBOT_OMNI}
    src/mbot_controller.c
    src/mbot_odometry.c
    src/mbot_print.c
    src/mbot_comms.c
    src/mbot_omni.c
  )
  configure_mbot(${MBOT_OMNI})
  target_compile_definitions(${MBOT_OMNI} PRIVATE MBOT_TYPE_OMNI)

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
  target_compile_definitions(${MBOT_CALIBRATE_OMNI} PRIVATE MBOT_TYPE_OMNI)
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
  target_compile_definitions(mbot_omni_motor_test PRIVATE MBOT_TYPE_OMNI)
  pico_enable_stdio_usb(mbot_omni_motor_test 1)
  pico_enable_stdio_uart(mbot_omni_motor_test 0)
  pico_add_extra_outputs(mbot_omni_motor_test)
endif()

if(NOT MBOT_TYPE OR MBOT_TYPE STREQUAL "BALBOT")
  # MBOT Balbot
  set(MBOT_BALBOT mbot_balbot${NAME_SUFFIX})
  add_executable(${MBOT_BALBOT}
    src/mbot_comms.c
    src/mbot_balbot.c
  )
  configure_mbot(${MBOT_BALBOT})
  target_compile_definitions(${MBOT_BALBOT} PRIVATE MBOT_TYPE_BALBOT)
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