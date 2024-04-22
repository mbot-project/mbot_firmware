# Manually set the mbot version
set(OMNI_VERSION "2.0")
set(CLASSIC_VERSION "1.0")

# Remove the dot and convert to integer directly for simplicity
string(REPLACE "." "" OMNI_VERSION_INT ${OMNI_VERSION})
string(REPLACE "." "" CLASSIC_VERSION_INT ${CLASSIC_VERSION})

add_definitions(-DOMNI_VERSION=${OMNI_VERSION_INT})
add_definitions(-DCLASSIC_VERSION=${CLASSIC_VERSION_INT})

function(configure_mbot target)
    target_link_libraries(${target}
      pico_stdlib
      pico_multicore
      pico_time
      hardware_pwm
      hardware_sync
      hardware_i2c
      hardware_flash
      mbotlib
      rclib
      comms
    )
        
    pico_enable_stdio_usb(${target} 1)
    pico_enable_stdio_uart(${target} 0)
    pico_add_extra_outputs(${target})
endfunction()

include_directories(/usr/local/include)

# MBOT Classic
add_executable(mbot_classic
  mbot_controller.c
  mbot_odometry.c
  mbot_print.c
  mbot_comms.c
  mbot_classic.c
)
configure_mbot(mbot_classic)

# MBOT Omni
add_executable(mbot_omni
  mbot_controller.c
  mbot_odometry.c
  mbot_print.c
  mbot_comms.c
  mbot_omni.c
)
configure_mbot(mbot_omni)