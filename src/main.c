

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#include "bsp/board_api.h"
#include "tusb.h"


//#include "pico/gpio.h"

#define BUTTON_PIN 20
#define LED_PIN 22
#define ADC_PIN 26


void led_setup(void); 
void led_blinking_task(void);
void adc_setup(void);
void adc_task(void);
void midi_task(void);

/*------------- MAIN -------------*/
int main(void)
{
    led_setup();
    adc_setup();
    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);


    while (1)
    {
        tud_task(); // tinyusb device task
        led_blinking_task();
        adc_task();
    }
}

long map(long x, long in_min, long in_max, long out_min, long out_max)

    {

      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    }

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// MIDI Task
//--------------------------------------------------------------------+

/*void midi_task(void)
{
    return;
    static uint32_t start_ms = 0;

    uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
    uint8_t const channel   = 0; // 0 for channel 1

    // The MIDI interface always creates input and output port/jack descriptors
    // regardless of these being used or not. Therefore incoming traffic should be read
    // (possibly just discarded) to avoid the sender blocking in IO
    uint8_t packet[4];
    while ( tud_midi_available() ) tud_midi_packet_read(packet);

    // send note periodically
    if (board_millis() - start_ms < 286) return; // not enough time
    start_ms += 286;

    // Previous positions in the note sequence.
    int previous = (int) (note_pos - 1);

    // If we currently are at position 0, set the
    // previous position to the last note in the sequence.
    if (previous < 0) previous = sizeof(note_sequence) - 1;

    // Send Note On for current position at full velocity (127) on channel 1.
    uint8_t note_on[3] = { 0x90 | channel, note_sequence[note_pos], 127 };
    tud_midi_stream_write(cable_num, note_on, 3);

    // Send Note Off for previous note.
    uint8_t note_off[3] = { 0x80 | channel, note_sequence[previous], 0};
    tud_midi_stream_write(cable_num, note_off, 3);

    // Increment position
    note_pos++;

    // If we are at the end of the sequence, start over.
    if (note_pos >= sizeof(note_sequence)) note_pos = 0;
}*/

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
void led_blinking_task(void)
{
    button_state_last = button_state;
    button_state = gpio_get(BUTTON_PIN);

    uint8_t packet[4];
    while ( tud_midi_available() ) tud_midi_packet_read(packet);

    if ( button_state && !button_state_last )
    {
        gpio_put( LED_PIN, !gpio_get(LED_PIN ) );

        // Send Note On for current position at full velocity (127) on channel 1.
        uint8_t note_on[3] = { 0x90 | 0, 80, 127 };
        tud_midi_stream_write( 0, note_on, 3);
    }

}

void adc_setup(void)
{
    adc_init();
    adc_gpio_init( ADC_PIN );

    //gpio_init(11);
    //gpio_set_dir(11, true );
    //gpio_put(11, true);
    //sleep_ms(500);
    //gpio_put(11, false);
    //gpio_deinit(11);

    gpio_set_function( 11, GPIO_FUNC_PWM );
    uint slice_num = pwm_gpio_to_slice_num(11);

    pwm_set_wrap(slice_num, 127);

    pwm_set_enabled( slice_num, true );
    
    pwm_set_gpio_level( 11, 127 );
    //sleep_ms(2000);
    //pwm_set_gpio_level( 11, 127/2 );
    //sleep_ms(2000);
    //pwm_set_gpio_level( 11, 128 );
    //sleep_ms(2000);
    //pwm_set_gpio_level( 11, 128 );
    //sleep_ms(5000);

    

    for ( uint16_t i = 0; i <= 127; i++ )
    {
        pwm_set_gpio_level( 11, i );
        sleep_ms( 10 );
    }
}

uint16_t result_0_127 = 0;
uint16_t result_0_127_pref = 0;
void adc_task(void)
{
    gpio_put( LED_PIN, 1 );
    
    gpio_put( LED_PIN, !gpio_get(LED_PIN ) );
    adc_select_input( 0 );
    uint16_t adc_result = adc_read();
    result_0_127_pref = result_0_127;
    result_0_127 = adc_result >> (12-7);

    pwm_set_gpio_level( 11, result_0_127 );
    //pwm_set_gpio_level( 11, map(adc_result, 0, 65535, 0, 127 ) );
    if ( result_0_127_pref != result_0_127 )
    {
        uint8_t note_on[3] = { 0x90 | 0, 81, result_0_127 };
        tud_midi_stream_write( 0, note_on, 3);
    }
}



