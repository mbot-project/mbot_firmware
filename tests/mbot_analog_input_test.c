#include <stdio.h>
#include <inttypes.h>
#include <mbot/barometer/barometer.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/adc.h>

int main()
{
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    adc_gpio_init(29);

    // 12-bit conversion, assume max value == ADC_VREF == 3.0 V
    // on MBot control board Vref is set by LM4040 Zener diode   
    const float conversion_factor = 3.0f / (1 << 12);
    sleep_ms(3000);
    printf(" AIN0           | AIN1           | AIN2           | AIN3             \n");
    printf(" Raw      Volts | Raw      Volts | Raw      Volts | Raw    Battery Volts\n");
    while (1) {
        adc_select_input(0);
        uint16_t ain0_result = adc_read();
        float ain0_conv = ain0_result * conversion_factor;
        adc_select_input(1);
        uint16_t ain1_result = adc_read();
        float ain1_conv = ain1_result * conversion_factor;
        adc_select_input(2);
        uint16_t ain2_result = adc_read();
        float ain2_conv = ain2_result * conversion_factor;
        adc_select_input(3);
        uint16_t ain3_result = adc_read();
        float ain3_conv = 5.0 * ain3_result * conversion_factor;
       printf(" 0x%03x  %6.2fV | 0x%03x  %6.2fV | 0x%03x  %6.2fV | 0x%03x  %6.2fV\r",
           ain0_result, ain0_conv,
           ain1_result, ain1_conv,
           ain2_result, ain2_conv,
           ain3_result, ain3_conv);

        sleep_ms(500);
    }
}