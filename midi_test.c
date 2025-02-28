
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

#pragma endregion

//#include "pico/gpio.h"

#pragma region defines

#define BUTTON_PIN 20


#pragma endregion

void led_setup(void); 
//void rot_sw_setup(void);

void button_task(void);

uint ctr = 0; 


/*------------- MAIN -------------*/
int main(void)
{
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
        process_midi_message(); 

        tud_task(); // tinyusb device task
        button_task();
        adc_task();
        rot_sw_task();
    }
}



//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_setup(void)
{
    gpio_init( LED_PIN );
    gpio_set_dir( LED_PIN, GPIO_OUT );
    gpio_put( LED_PIN, false );

    gpio_init( BUTTON_PIN );
    gpio_set_dir( BUTTON_PIN, GPIO_IN );
    gpio_pull_up(BUTTON_PIN);
}
bool button_state = 0;
bool button_state_last = 0;
void button_task(void)
{
    button_state_last = button_state;
    button_state = gpio_get(BUTTON_PIN);

    uint8_t packet[4] = { 0, 0, 0, 0 };
    
    //tud_midi_packet_read(packet);
    //for( int i = 0; i < 4; i++ )
    //{
    //    if ( packet[i] != 0 )
    //    {
    //        static uint debug = 0;
    //        debug++;
    //    }
    //}

    if ( button_state && !button_state_last )
    {
        gpio_put( LED_PIN, !gpio_get(LED_PIN ) );

        // Send Note On for current position at full velocity (127) on channel 1.
        uint8_t note_on[3] = { 0x90 | 0, 80, 127 };
        tud_midi_stream_write( 0, note_on, 3);
    }

}


