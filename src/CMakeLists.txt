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

    if(${target} STREQUAL "mbot_classic")
        target_compile_definitions(${target} PRIVATE -DMBOT_DRIVE_TYPE=DIFFERENTIAL_DRIVE)
    endif()

    if(${target} STREQUAL "mbot_omni")
        target_compile_definitions(${target} PRIVATE -DMBOT_DRIVE_TYPE=OMNI_120_DRIVE)
    endif()
        
    pico_enable_stdio_usb(${target} 1)
    pico_enable_stdio_uart(${target} 0)
    pico_add_extra_outputs(${target})
endfunction()

include_directories(/usr/local/include)

# MBOT Classic
add_executable(mbot_classic
  mbot.c
  mbot_controller.c
  mbot_odometry.c
  mbot_print.c
  mbot_comms.c
  mbot_classic.c
)
configure_mbot(mbot_classic)

# MBOT Omni
add_executable(mbot_omni
  mbot.c
  mbot_controller.c
  mbot_odometry.c
  mbot_print.c
  mbot_comms.c
  mbot_omni.c
)
configure_mbot(mbot_omni)