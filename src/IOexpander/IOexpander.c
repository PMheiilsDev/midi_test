
#include "IOexpander.h"

bool IOexpander_init(void)
{
    i2c_init(I2C_PORT, I2C_SPEED);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Test if PCF8575 is connected
    while(1)
    {
        // this makes all leds blink very fast 
        uint8_t test_data[2] = {0x00, 0x00};
        int result = i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR, test_data, 2, false);
        uint8_t test_data2[2] = {0xFF, 0xFF};
        result = i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR, test_data2, 2, false);
    }
    return false;
    // if (result == PICO_ERROR_GENERIC) 
    // {
    //     return false; 
    // }
    // else
    // {
    //     //volatile uint8_t buffer[100];
    //     //volatile int result = i2c_read_blocking(I2C_PORT, IO_EXPANDER_ADDR, buffer, 50, false);
    //     //__breakpoint();
    // }

    return true; 
}
