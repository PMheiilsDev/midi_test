
#include "button.h"

#include <inttypes.h>
#include <stdbool.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"


bool button_send_midi_active[BUTTON_AMT] = 
{
    true
};

uint8_t button_midi_num[BUTTON_AMT] = 
{
    0
};

bool button_is_push_button[BUTTON_AMT] = 
{
    true
};

uint8_t button_gpio_num[BUTTON_AMT] = 
{
    20
};



static inline void button_init()
{
    // init all the gpio's as Outputs
    for ( int i = 0; i < BUTTON_AMT; i++ ) 
    {
        gpio_init( i );
        gpio_set_dir( button_get_gpio(i), GPIO_OUT );
    }
    
}

static uint8_t button_get_gpio( uint8_t i )
{
    return button_gpio_num[i];
}

static uint8_t button_get_midi_num( uint8_t i )
{
    return button_midi_num[i];
}

bool button_get_state( uint8_t i )
{
    return gpio_get(i);
}

bool button_get_state_midi_number(uint8_t)
{
    return 0; // not implemented
}

bool button_get_state_gpio( uint8_t i )
{
    return 0; // not implemented
}

