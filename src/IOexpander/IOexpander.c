
#include "IOexpander.h"

bool IOexpander_init(void)
{
    i2c_init(I2C_PORT, I2C_SPEED);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Test if PCF8575 is connected
    uint8_t test_data2[2] = {0xFF, 0xFF};
    int result = i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR, test_data2, 2, false);
    if (result == PICO_ERROR_GENERIC) 
    {
        return false; 
    }
    else
    {
        while(1)
        {
            volatile uint8_t buffer[2];
            volatile int result = i2c_read_blocking(I2C_PORT, IO_EXPANDER_ADDR, (uint8_t*) buffer, 2, false);
            if (result == PICO_ERROR_GENERIC) 
            {
                return false; 
            }
        }
    }

    return true; 
}
