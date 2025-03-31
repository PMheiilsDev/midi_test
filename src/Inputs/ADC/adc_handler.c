#include "adc_handler.h"

#include <stdlib.h>

adc_channel_t adc_channels[MAX_ADC_CHANNELS] = 
{
    {
        .pin = 26,
        .note = 81,
        .is_mul_plex = true, 
        .mul_plex_channel = 0,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    },
    {
        .pin = 26,
        .note = 82,
        .is_mul_plex = true, 
        .mul_plex_channel = 1,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    },
    {
        .pin = 26,
        .note = 83,
        .is_mul_plex = true, 
        .mul_plex_channel = 2,
        .mul_plex = {ADC_MUL_PLEX_GPIO_0, ADC_MUL_PLEX_GPIO_1, ADC_MUL_PLEX_GPIO_2},
        .result = 0,
        .num_reads = 20
    },
    {
        .pin = 27,
        .note = 84,
        .is_mul_plex = false, 
        .result = 0,
        .num_reads = 20
    }
};

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

        memset(adc_channels[i].result_pref, 0, sizeof(adc_channels[i].result_pref));
    }
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
uint8_t find_min(const uint8_t *arr, size_t size) 
{
    if (size == 0) 
    {
        return 0;  // Handle empty array case (optional, depends on use case)
    }

    uint8_t min_value = arr[0];  // Assume first element is the smallest

    for (size_t i = 1; i < size; i++) 
    {
        if (arr[i] < min_value) 
        {
            min_value = arr[i];
        }
    }

    return min_value;
}
uint8_t find_average(const uint8_t *arr, size_t size) {
    if (size == 0) {
        return 0.0;  // Handle empty array case
    }

    uint32_t sum = 0;  // Use uint32_t to prevent overflow

    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }

    return sum / size;
}
uint8_t debug_out = 0;
void adc_task(void)
{
    /// NOTE: all this is bull shit just check if the 4096 (12 bit value is changed by a bit less than (1>>7)<<12) [[maybe half ???]] 

    for (int i = 0; i < MAX_ADC_CHANNELS; i++) 
    {
        if ( adc_channels[i].is_mul_plex ) 
        {
            adc_set_mul_plex_gpios(&adc_channels[i]);
        }

        adc_select_input(gpio_to_adc_channel(adc_channels[i].pin));

        uint32_t adc_sum = 0;
        for (int j = 0; j < adc_channels[i].num_reads; j++) 
        {
            adc_sum += adc_read();
            //sleep_us(100);
        }

        uint16_t result = adc_sum / adc_channels[i].num_reads;

        adc_channels[i].result_pref[adc_channels[i].result_pref_ctr] = result * 128 / 4095;
        // adc_channels[i].result_pref[adc_channels[i].result_pref_ctr] = 100 + rand()%2;
        
        adc_channels[i].result_pref_ctr++;

        adc_channels[i].result = find_average(adc_channels[i].result_pref, sizeof(adc_channels[i].result_pref));

        if ( adc_channels[i].result_pref_send != adc_channels[i].result ) 
        {
            adc_channels[i].result_pref_send = adc_channels[i].result;

            uint8_t note_on[3] = {0b10110000 | 0, adc_channels[i].note, adc_channels[i].result }; 
            tud_midi_stream_write(0, note_on, 3);
        }
    }
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

