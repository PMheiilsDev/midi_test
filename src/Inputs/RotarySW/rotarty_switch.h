#ifndef _ROT_SW_H
#define _ROT_SW_H


#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "hardware/gpio.h"
#include "tusb.h"



#define ROT_SW_CLK_PIN 2
#define ROT_SW_DATA_PIN 3


void rot_sw_setup(void);


void sw_interupt_callback();


void rot_sw_task(void);

#endif 