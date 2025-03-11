
#include "IOexpander.h"

uint16_t IOexpander_pin_func;
uint16_t IOexpander_pin_func_pref;
uint16_t IOexpander_pin_state;
uint16_t IOexpander_pin_state_pref;



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

    //! MISSING turn all pins to high impedance 

    return true; 
}






void IOexpander_set_function( uint8_t pin_num, IOexpander_pin_func_t value )
{
    IOexpander_pin_func |= ( value&1 << pin_num ); 
}


void IOexpander_put( uint8_t pin_num, IOexpander_pin_output_state_t value )
{
    IOexpander_pin_state |= ( value&1 << pin_num );
}


IOexpander_pin_func_t IOexpander_get_function( uint8_t pin_num )
{
    // this does not use the enums just leave it this way 
    return (IOexpander_pin_func >> pin_num) & 1; 
}





bool IOexpander_write( bool force )
{
    if ( force || ( IOexpander_pin_func != IOexpander_pin_func_pref ) || ( IOexpander_pin_state != IOexpander_pin_state_pref ) )
    {
        // if the pin is input set to 1 or if the pin is highz set to 1 
        uint16_t data = IOexpander_pin_func | IOexpander_pin_state;

        i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR, &data, sizeof(data), false);
        
        IOexpander_pin_func_pref = IOexpander_pin_func;
        IOexpander_pin_state_pref = IOexpander_pin_state;

        return true;
    }
    return false;
}



