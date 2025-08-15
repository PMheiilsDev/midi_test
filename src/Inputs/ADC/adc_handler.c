#include "adc_handler.h"

#include <stdlib.h>
#include "pico/time.h"

adc_channel_t adc_channels[MAX_ADC_CHANNELS] = 
{
    {
        .pin = 26,
        .note = 80,
        .is_mul_plex = true, 
        .mul_plex_channel = 0,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    },
    {
        .pin = 26,
        .note = 81,
        .is_mul_plex = true, 
        .mul_plex_channel = 1,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    },
    {
        .pin = 26,
        .note = 82,
        .is_mul_plex = true, 
        .mul_plex_channel = 2,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    },
    {
        .pin = 26,
        .note = 83,
        .is_mul_plex = true, 
        .mul_plex_channel = 3,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    },
    {
        .pin = 26,
        .note = 84,
        .is_mul_plex = true, 
        .mul_plex_channel = 4,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    }
};

uint8_t current_adc_channel = 0;

repeating_timer_t adc_timer;

void adc_setup(void)
{
    adc_init();

    for (int i = 0; i < MAX_ADC_CHANNELS; i++) 
    {
        adc_gpio_init(adc_channels[i].pin);

        if ( adc_channels[i].is_mul_plex ) 
        {
            for ( uint j = 0; j < sizeof( adc_channels[i].mul_plex ); j++ ) 
            {
                gpio_set_function(adc_channels[i].mul_plex[j], GPIO_FUNC_SIO);
                gpio_put(adc_channels[i].mul_plex[j], 0);
                gpio_set_dir(adc_channels[i].mul_plex[j], GPIO_OUT);
            }
        }
        adc_channels[i].result_pref = 0;
        //memset(adc_channels[i].result_pref, 0, sizeof(adc_channels[i].result_pref));
    }

    add_repeating_timer_us(-2500, adc_task, NULL, &adc_timer);
}

static uint8_t val_in_array( uint8_t *arr, uint8_t val, uint8_t size )
{
    uint8_t ctr = 0;
    for ( int i = 0; i < size; i++ ) 
    {
        if ( arr[i] == val ) 
        {
            ctr++;
        }
    }
    return ctr;
}

bool adc_task( repeating_timer_t *rt )
{

    
    if ( adc_channels[current_adc_channel].is_mul_plex ) 
    {
        adc_set_mul_plex_gpios(&adc_channels[current_adc_channel]);
    }

    adc_select_input(gpio_to_adc_channel(adc_channels[current_adc_channel].pin));

    uint32_t adc_sum = 0;
    for (int j = 0; j < 1+0*adc_channels[current_adc_channel].num_reads; j++) 
    {
        adc_sum += adc_read();
        // sleep_us(100);
    }

    adc_channels[current_adc_channel].result = adc_sum/1+0*adc_channels[current_adc_channel].num_reads;

    if ( abs(adc_channels[current_adc_channel].result_pref - adc_channels[current_adc_channel].result) >= (1<<(12-7))/4 ) // the /4 is there so that at the edges it alway reaches 127 / 0 and does not stop one before that
    {
        adc_channels[current_adc_channel].result_pref = adc_channels[current_adc_channel].result;
        adc_channels[current_adc_channel].res_7_bit = adc_channels[current_adc_channel].result>>(12-7);

        uint8_t note_on[3] = {0b10110000 | 0, adc_channels[current_adc_channel].note, adc_channels[current_adc_channel].res_7_bit }; 
        tud_midi_stream_write(0, note_on, 3);
    }
    
    current_adc_channel++;
    if (current_adc_channel >= MAX_ADC_CHANNELS) 
        current_adc_channel = 0;

    return true;
}



static inline int gpio_to_adc_channel(uint gpio)
{
    switch (gpio)
    {
        case 26: return 0;
        case 27: return 1;
        case 28: return 2;
        case 29: return 3;
        default: return -1; // Invalid GPIO for ADC
    }
}

void adc_set_mul_plex_gpios( adc_channel_t *adc_channel )
{
    // for all the gpio pins in the channel 
    for ( uint i = 0; i < sizeof( adc_channel->mul_plex ); i++ ) 
    {
        // set the gpio pin to the correct bit of the channel number
        gpio_put(adc_channel->mul_plex[i], (adc_channel->mul_plex_channel >> i) & 1);

        // here might be some delay needed (minimise)  
        //sleep_us(10);
    }
}

