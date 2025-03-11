
#pragma region includes

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#include "bsp/board_api.h"
#include "tusb.h"

#include "rotarty_switch.h"
#include "midi_handler.h"
#include "adc_handler.h"
#include "led_handler.h"
#include "ws2812_handler.h"
#include "IOexpander.h"

#include "pico/time.h"



#pragma endregion

//#include "pico/gpio.h"

#pragma region defines



#pragma endregion

//void rot_sw_setup(void);

void button_task(void);

uint ctr = 0; 

uint64_t loop_time = 0;
uint64_t loop_time_pref = 0;

/*------------- MAIN -------------*/
int main(void)
{
    ws2812_init();

    IOexpander_init();

    // LED should be on 
    IOexpander_set_function(0, IO_EXPANDER_PIN_FUNC_OUTPUT);
    IOexpander_put(0, IO_EXPANDER_PIN_OUTPUT_STATE_LOW);
    
    // LED should be off
    IOexpander_set_function(1, IO_EXPANDER_PIN_FUNC_OUTPUT);
    IOexpander_put(1, IO_EXPANDER_PIN_OUTPUT_STATE_HIGH_Z);

    // LED should be off
    IOexpander_set_function(2, IO_EXPANDER_PIN_FUNC_INPUT);
    IOexpander_put(2, IO_EXPANDER_PIN_OUTPUT_STATE_LOW);

    // LED should be off
    IOexpander_set_function(3, IO_EXPANDER_PIN_FUNC_INPUT);
    IOexpander_put(3, IO_EXPANDER_PIN_OUTPUT_STATE_HIGH_Z);
    
    
    IOexpander_write(true);




    for ( uint i = 0; i < 16; i++ )
    {
        IOexpander_set_function(i, IO_EXPANDER_PIN_FUNC_OUTPUT);
        IOexpander_put(i, IO_EXPANDER_PIN_OUTPUT_STATE_LOW);
        IOexpander_write(true);
    }
    for ( uint i = 0; i < 16; i++ ) 
    {
        IOexpander_set_function(i, IO_EXPANDER_PIN_FUNC_OUTPUT);
        IOexpander_put(i, IO_EXPANDER_PIN_OUTPUT_STATE_HIGH_Z);
        IOexpander_write(true);
    }
    bool d = false;
    for ( uint i = 0; i < 16; i++ ) 
    {
        IOexpander_set_function(i, IO_EXPANDER_PIN_FUNC_INPUT);
        d = IOexpander_get( i, true );
        printf("pin %d is %d\n", i, d);
    }
    //IOexpander_set_function(10, IO_EXPANDER_PIN_FUNC_OUTPUT);
    //IOexpander_put(10, IO_EXPANDER_PIN_OUTPUT_STATE_LOW);

    //IOexpander_write(true);

    led_setup();
    adc_setup();
    rot_sw_setup();
    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    char data [640*4];
    char temp[12];
    sprintf(temp,"%x",data);
    for( int i = 0; i < 640*4; i++ )
    {
        data[i] = 0; 
    }

    buffer = malloc( 0x100 * 128 * sizeof(char) );

    
    while (1)
    {
        loop_time_pref = loop_time;
        loop_time = time_us_64();
        
        process_midi_message(); 

        tud_task(); // tinyusb device task
        button_task();
        adc_task();
        rot_sw_task();
    }
}




