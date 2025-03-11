
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

    IOexpander_pin_func = 0xFFFF;
    IOexpander_pin_state = 0xFFFF;

    // Test if PCF8575 is connected
    uint8_t data[2] = {0xFF, 0xFF};
    int result = i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR, data, 2, false);

    return (result == PICO_ERROR_GENERIC); 
}






void IOexpander_set_function( uint8_t pin_num, IOexpander_pin_func_t value )
{
    IOexpander_pin_func = (IOexpander_pin_func & ~(1 << pin_num)) | ((value & 1) << pin_num);

}


void IOexpander_put( uint8_t pin_num, IOexpander_pin_output_state_t value )
{
    IOexpander_pin_state = (IOexpander_pin_state & ~(1 << pin_num)) | ((value & 1) << pin_num);
    
}


IOexpander_pin_func_t IOexpander_get_function( uint8_t pin_num )
{
    // this does not use the enums just leave it this way 
    return (IOexpander_pin_func >> pin_num) & 1; 
}

static uint8_t reverse_bits(uint8_t byte)
{
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

bool IOexpander_write( bool force )
{
    if ( force || ( IOexpander_pin_func != IOexpander_pin_func_pref ) || ( IOexpander_pin_state != IOexpander_pin_state_pref ) )
    {
        // if the pin is input set to 1 or if the pin is highz set to 1 
        uint8_t data [2];
        data[0] = ( ( IOexpander_pin_func | (~IOexpander_pin_func & IOexpander_pin_state) ) >> 0 ) & 0xFF;
        data[1] = ( ( IOexpander_pin_func | (~IOexpander_pin_func & IOexpander_pin_state) ) >> 8 ) & 0xFF;

        i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR, data, sizeof(data), false);
        
        IOexpander_pin_func_pref = IOexpander_pin_func;
        IOexpander_pin_state_pref = IOexpander_pin_state;

        return true;
    }
    return false;
}


bool IOexpander_read()
{
    uint8_t data [2];
    i2c_read_blocking(I2C_PORT, IO_EXPANDER_ADDR, data, sizeof(data), false);

    IOexpander_pin_state = ((data[1] << 8) | data[0]) /*& IOexpander_pin_func*/;

    bool ret = IOexpander_pin_state != IOexpander_pin_state_pref;

    IOexpander_pin_state_pref = IOexpander_pin_state;

    return ret;
}


IOexpander_pin_input_state_t IOexpander_get( uint8_t pin_num, bool force_update )
{
    if ( force_update )
    {
        IOexpander_read();
    }


    bool value = IOexpander_pin_state >> pin_num & 1;
    
    return value;
}


