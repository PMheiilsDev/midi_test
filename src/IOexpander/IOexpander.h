#ifndef _IO_EXPANDER_H
#define _IO_EXPANDER_H

#include "hardware/i2c.h"
#include "hardware/gpio.h"

#define I2C_PORT i2c0 
#define I2C_SPEED 400000 

#define I2C_SDA 16 
#define I2C_SCL 17 

#define IO_EXPANDER_ADDR 0x20 

// for the PCF8575 writing two bytes sets the state of the pins 
// if a pin is 0 then the  pins connects to GND with a small resistance
// if a pin is 1 then the pin is high impedance 

// if a pin is 1 it can be used as input 
// meaning that the read bit of that pin will be 0 if connected to GND and else 1 (as it has a pull up resistor)

//? ( dont make these extern no access to them from out side )
//? extern uint16_t IOexpander_pin_func;
//? extern uint16_t IOexpander_pin_func_pref;
//? 
//? extern uint16_t IOexpander_pin_state;
//? extern uint16_t IOexpander_pin_state_pref;

typedef enum 
{
    IO_EXPANDER_PIN_FUNC_OUTPUT = 0,
    IO_EXPANDER_PIN_FUNC_INPUT = 1,
}
IOexpander_pin_func_t;

typedef enum 
{
    IO_EXPANDER_PIN_OUTPUT_STATE_LOW = 0,
    IO_EXPANDER_PIN_OUTPUT_STATE_HIGH_Z = 1,
}
IOexpander_pin_output_state_t;

typedef enum 
{
    IO_EXPANDER_PIN_INPUT_STATE_ERROR = -1,
    IO_EXPANDER_PIN_INPUT_STATE_LOW = 0,
    IO_EXPANDER_PIN_INPUT_STATE_HIGH = 1,
}
IOexpander_pin_input_state_t;

/// @brief initializes the IOexpander 
/// @return true if the IOexpander is connected and false if not 
bool IOexpander_init();

/// @brief sets the function of a ioexpander pin 
/// @param pin_num number of the pin 0 to 15
/// @param value from enum IOexpander_pin_func 
void IOexpander_set_function( uint8_t pin_num, IOexpander_pin_func_t value );

/// @brief get the function of a pin 
/// @param pin_num number of the pin 0 to 15 
/// @return 
IOexpander_pin_func_t IOexpander_get_function( uint8_t pin_num );

/// @brief puts a value on a ioexpander pin (after calling IOexpander_update())
/// @param pin_num number of the pin 0 to 15 
/// @param value from enum IOexpander_pin_state 
void IOexpander_put( uint8_t pin_num, IOexpander_pin_output_state_t value );

/// @brief gets the value of a IO expander input pin
/// @param pin_num number of the pin 0 to 15 
/// @param force_update true to update the IO expander via I2C before reading in this function 
/// @return IOexpander_pin_input_state_t (error if pin is Output pin)
IOexpander_pin_input_state_t IOexpander_get( uint8_t pin_num, bool force_update );

/// @brief updates the IO expander via I2C (only if changes are done)
/// @param force force to update even if no changes where done (true recomended at first write)
/// @return true if changes where done and false if values where already correct 
bool IOexpander_write( bool force );

/// @brief updates the input pin values of the IO expander via I2C  
/// @return true if anything changed 
bool IOexander_read();

#endif

