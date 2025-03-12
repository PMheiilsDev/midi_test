
#include "IOexpander.h"
#include "rotarty_switch.h"

// this is not ideal but it will work 
// better would be to have one callback that uses else if so that if one gpio was correct none others are tested (should be good enough though)
void main_interupt_callback(uint gpio, uint32_t events);


