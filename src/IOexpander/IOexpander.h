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


bool IOexpander_init(void);


#endif

