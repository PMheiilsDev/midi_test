#ifndef _ROT_SW_H
#define _ROT_SW_H

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "tusb.h"

#define MAX_ROTARY_SWITCHES 4

typedef struct 
{
    uint8_t clk_pin;
    uint8_t data_pin;
    uint8_t note;
    volatile uint8_t counter;
    volatile uint8_t last_sent;
} rotary_switch_t;

extern rotary_switch_t rotary_switches[MAX_ROTARY_SWITCHES];

void rot_sw_setup(void);
void sw_interupt_callback(uint gpio, uint32_t events);
void rot_sw_task(void);

#endif