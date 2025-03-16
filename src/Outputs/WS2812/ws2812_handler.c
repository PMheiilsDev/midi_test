
#include "ws2812_handler.h"



WS_2812_t WS_2812[WS2812_AMT] = 
{
    // Rotary Switches 
    {
        .limit = 127,
        .value = &(rotary_switches[0].counter),
    },
    {
        .limit = 127,
        .value = &(rotary_switches[1].counter),
    },
    {
        .limit = 127,
        .value = &(rotary_switches[2].counter),
    },
    {
        .limit = 127,
        .value = &(rotary_switches[3].counter),
    },

    // ADCs
    {
        .limit = 127,
        .value = &(adc_channels[0].result),
    },
    {
        .limit = 127,
        .value = &(adc_channels[1].result),
    },
    {
        .limit = 127,
        .value = &(adc_channels[2].result),
    },
    {
        .limit = 127,
        .value = &(adc_channels[3].result),
    },
};


