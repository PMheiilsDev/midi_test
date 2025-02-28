
#include "adc_handler.h"




void adc_setup(void)
{
    adc_init();
    adc_gpio_init( ADC_PIN );


    gpio_set_function( PWM_PIN, GPIO_FUNC_PWM );
    uint slice_num = pwm_gpio_to_slice_num(11);

    pwm_set_wrap(slice_num, 127);

    pwm_set_enabled( slice_num, true );
    
    pwm_set_gpio_level( PWM_PIN, 127 );
    

    for ( uint16_t i = 0; i <= 127; i++ )
    {
        pwm_set_gpio_level( PWM_PIN, i );
        sleep_ms( 10 );
    }
}

uint16_t result_0_127 = 0;
uint16_t result_0_127_pref = 0;
uint16_t adc_result;
void adc_task(void)
{
    gpio_put( LED_PIN, 1 );
    
    gpio_put( LED_PIN, !gpio_get(LED_PIN ) );
    adc_select_input( 1 );
    /*uint16_t*/ adc_result = adc_read();
    
    // change this "mapping" so that all 
    if( adc_result <= 200 )
    {
        result_0_127 = 0;
    }
    else
    {
        //result_0_127 = adc_result >> (12-7);
        result_0_127 = ((adc_result - 201) * (127 - 60)) / (4096 - 201) + 60;
    }

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



