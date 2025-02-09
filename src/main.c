
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

#pragma endregion

//#include "pico/gpio.h"

#pragma region defines

#define BUTTON_PIN 20
#define LED_PIN 22
#define ADC_PIN 26
#define PWM_PIN 11

#define ROT_SW_CLK_PIN 2
#define ROT_SW_DATA_PIN 3

#pragma endregion

void led_setup(void); 
void adc_setup(void);
void rot_sw_setup(void);

void led_blinking_task(void);
void adc_task(void);
void midi_task(void);
void rot_sw_task(void);
void rot_sw_task(void);

/*------------- MAIN -------------*/
int main(void)
{
    led_setup();
    adc_setup();
    rot_sw_setup();
    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);


    while (1)
    {
        tud_task(); // tinyusb device task
        led_blinking_task();
        adc_task();
        rot_sw_task();
    }
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+
#pragma region tud_functions

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

#pragma endregion

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

    gpio_set_function( PWM_PIN, GPIO_FUNC_PWM );
    uint slice_num = pwm_gpio_to_slice_num(11);

    pwm_set_wrap(slice_num, 127);

    pwm_set_enabled( slice_num, true );
    
    pwm_set_gpio_level( PWM_PIN, 127 );
    //sleep_ms(2000);
    //pwm_set_gpio_level( 11, 127/2 );
    //sleep_ms(2000);
    //pwm_set_gpio_level( 11, 128 );
    //sleep_ms(2000);
    //pwm_set_gpio_level( 11, 128 );
    //sleep_ms(5000);

    

    for ( uint16_t i = 0; i <= 127; i++ )
    {
        pwm_set_gpio_level( PWM_PIN, i );
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
    
    result_0_127 = adc_result >> (12-7);

    //pwm_set_gpio_level( PWM_PIN, result_0_127 );
    //pwm_set_gpio_level( PWM_PIN, map(adc_result, 0, 65535, 0, 127 ) );
    long difference = (long)(result_0_127_pref - result_0_127);
    if ( difference > 1 || difference < -1 ) 
    {
        result_0_127_pref = result_0_127;
        uint8_t note_on[3] = { 0b10110000 | 0, 81, result_0_127 };
        tud_midi_stream_write( 0, note_on, 3);
    }
}
uint8_t sw_ctr = 0;
uint32_t time_of_last_clk = 0;
uint32_t time_of_last_clk_pref = 0;
void sw_interupt_callback()
{
    bool data = gpio_get(ROT_SW_DATA_PIN);

    //time_of_last_clk_pref = time_of_last_clk;
    //time_of_last_clk = to_ms_since_boot(get_absolute_time());
    //
    //if ( time_of_last_clk - time_of_last_clk_pref < 25) return;

    if ( data != 0 )
    {
        if ( sw_ctr > 0 )
        {
            sw_ctr -=2;
            //uint8_t note_on[3] = { 0b10110000 | 0, 82, 0x7F & sw_ctr };
            //tud_midi_stream_write( 0, note_on, 3);
        }
    }
    else
    {
        if ( sw_ctr < 126 )
        {
            sw_ctr +=2;
            //uint8_t note_on[3] = { 0b10110000 | 0, 82, 0x7F & sw_ctr };
            //tud_midi_stream_write( 0, note_on, 3);
        }
    }
    gpio_put(7,!gpio_get(7));
}
void rot_sw_setup(void)
{
    gpio_init(7);
    gpio_set_dir(7, GPIO_OUT);

    gpio_init(ROT_SW_DATA_PIN);
    gpio_set_dir(ROT_SW_DATA_PIN, GPIO_IN );
    gpio_pull_up(ROT_SW_DATA_PIN);

    gpio_init(ROT_SW_CLK_PIN);
    gpio_set_dir(ROT_SW_CLK_PIN, GPIO_IN );
    gpio_pull_up(ROT_SW_CLK_PIN);

    gpio_set_irq_enabled_with_callback(ROT_SW_CLK_PIN, GPIO_IRQ_EDGE_RISE, true, &sw_interupt_callback);
}
uint8_t sw_ctr_last_sent = 0;
void rot_sw_task(void)
{
    if ( sw_ctr_last_sent != sw_ctr )
    {
        sw_ctr_last_sent = sw_ctr;
        uint8_t note_on[3] = { 0b10110000 | 0, 82, 0x7F & sw_ctr };
        tud_midi_stream_write( 0, note_on, 3);
        pwm_set_gpio_level( PWM_PIN, sw_ctr );
    }
}