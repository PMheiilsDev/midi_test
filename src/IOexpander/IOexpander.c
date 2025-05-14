
#include "IOexpander.h"

uint16_t IOexpander_pin_func;
uint16_t IOexpander_pin_func_pref;
uint16_t IOexpander_pin_state;
uint16_t IOexpander_pin_state_pref;

bool IOexpander_int = false;

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
    int result_0 = i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR_0, &data[0], 1, false);
    int result_1 = i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR_1, &data[1], 1, false);

    if ( (result_0 != PICO_ERROR_GENERIC) && (result_1 != PICO_ERROR_GENERIC) )
    {
        gpio_init(IO_EXPANDER_INT_PIN);
        gpio_set_dir(IO_EXPANDER_INT_PIN, GPIO_IN);
        gpio_pull_up(IO_EXPANDER_INT_PIN);


        gpio_set_irq_enabled_with_callback(IO_EXPANDER_INT_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &main_interupt_callback);
    }

    // IOexpander_set_function( 5, IO_EXPANDER_PIN_FUNC_OUTPUT );

    return ((result_0 != PICO_ERROR_GENERIC) && (result_1 != PICO_ERROR_GENERIC)); 
}


void IO_expander_interupt_callback(uint gpio, uint32_t events)
{
    if( (gpio == IO_EXPANDER_INT_PIN) && (events & GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL) )
    {
        IOexpander_int = true;
    }
    return;
}


void IOexpander_task()
{
    IOexpander_write(false);
    
    if ( IOexpander_int )
    {
        IOexpander_read();
        IOexpander_int = false;
    }
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

        i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR_0, &data[0], 1, false);
        i2c_write_blocking(I2C_PORT, IO_EXPANDER_ADDR_1, &data[1], 1, false);
        
        IOexpander_pin_func_pref = IOexpander_pin_func;
        IOexpander_pin_state_pref = IOexpander_pin_state;

        return true;
    }
    return false;
}


bool IOexpander_read()
{
    uint8_t data [2];
    i2c_read_blocking(I2C_PORT, IO_EXPANDER_ADDR_0, &data[0], 1, false);
    i2c_read_blocking(I2C_PORT, IO_EXPANDER_ADDR_1, &data[1], 1, false);

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


void IOexpander_get_pin_types()
{
    static volatile int8_t button_pin_of_i_th_led_pin[16];

    for ( int i = 0; i < 16; i++ )
    {
        // pull down pin after pin to ground (turn on leds)
        for ( int j = 0; j < 16; j++ )
        {
            IOexpander_set_function( j, IO_EXPANDER_PIN_FUNC_INPUT );
            IOexpander_put( j, IO_EXPANDER_PIN_OUTPUT_STATE_HIGH_Z );
        }
        IOexpander_set_function( i, IO_EXPANDER_PIN_FUNC_OUTPUT );
        IOexpander_put( i, IO_EXPANDER_PIN_OUTPUT_STATE_LOW );
        
        IOexpander_write( true );

        // wait for 5 seconds and if int was triggered a button was pressed meaning that the last pin is a led 
        IOexpander_read();

        sleep_ms( 2000 );
        if ( IOexpander_int )
        {
            uint16_t temp = IOexpander_pin_state;
            IOexpander_read();
            IOexpander_int = false;
            
            button_pin_of_i_th_led_pin[i] = get_single_set_bit_index( temp ^IOexpander_pin_state );
        }
        
    }
    __breakpoint();
    // if leds_pins of a pin is still 0 then it is a button
    // if leds_pins of a pin is not 0 then it is a led
    // it is connected to the button by what amount of bits one is shifted to the left 
    // this is the result of the debugging
    // ledpin,buttonpin:
    // 0,1
    // 2,3
    // 4,6
    // 5,7

    // 9,8
    // 11,10
    // 13,12
    // 15,14

}


// temp
int get_single_set_bit_index(uint16_t x) 
{
    if (x == 0 || (x & (x - 1)) != 0) 
    {
        return -1;  // No bits or more than one bit is set
    }

    int index = 0;
    while ((x & 1) == 0) 
    {
        x >>= 1;
        index++;
    }
    return index;
}