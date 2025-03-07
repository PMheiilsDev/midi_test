#include "adc_handler.h"

adc_channel_t adc_channels[MAX_ADC_CHANNELS] = 
{
    {
        .pin = 26,
        .note = 81,
        .result = 0,
        .result_pref = 0
    },
    {
        .pin = 27,
        .note = 82,
        .result = 0,
        .result_pref = 0
    },
    {
        .pin = 28,
        .note = 83,
        .result = 0,
        .result_pref = 0
    },
    {
        .pin = 29,
        .note = 84,
        .result = 0,
        .result_pref = 0
    }
};

void adc_setup(void)
{
    adc_init();

    for (int i = 0; i < MAX_ADC_CHANNELS; i++) 
    {
        adc_gpio_init(adc_channels[i].pin);
    }
}

void adc_task(void)
{

    for (int i = 0; i < MAX_ADC_CHANNELS; i++) 
    {
        adc_select_input(gpio_to_adc_channel(adc_channels[i].pin));

        uint32_t adc_sum = 0;
        for (int j = 0; j < NUM_ADC_READS; j++) 
        {
            adc_sum += adc_read();
            sleep_us(100);
        }

        adc_channels[i].result = adc_sum / NUM_ADC_READS;

        if (adc_channels[i].result <= 200) 
        {
            adc_channels[i].result = 0;
        }
        else 
        {
            adc_channels[i].result = ((adc_channels[i].result - 201) * (127 - 60)) / (4095 - 201) + 60 + 1;
        }
        
        if (adc_channels[i].result_pref != adc_channels[i].result ) 
        {
            adc_channels[i].result_pref = adc_channels[i].result;
            uint8_t note_on[3] = {0b10110000 | 0, adc_channels[i].note, adc_channels[i].result};
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

