include_directories(/usr/local/include)

#Motor Calibration
add_executable(mbot_calibrate_classic
  mbot_calibrate_classic.c
)
target_link_libraries(mbot_calibrate_classic
  pico_stdlib
  hardware_i2c
  hardware_pio
  hardware_pwm
  rclib
)

pico_enable_stdio_usb(mbot_calibrate_classic 1)
pico_enable_stdio_uart(mbot_calibrate_classic 0)
pico_add_extra_outputs(mbot_calibrate_classic)

#Motor Calibration
add_executable(mbot_calibrate_omni
  mbot_calibrate_omni.c
)
target_link_libraries(mbot_calibrate_omni
  pico_stdlib
  hardware_i2c
  hardware_pio
  hardware_pwm
  rclib
)
pico_enable_stdio_usb(mbot_calibrate_omni 1)
pico_enable_stdio_uart(mbot_calibrate_omni 0)
pico_add_extra_outputs(mbot_calibrate_omni)