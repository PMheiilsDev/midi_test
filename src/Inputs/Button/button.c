
#include "button.h"


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


