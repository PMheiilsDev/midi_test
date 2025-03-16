
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


void ws2812_setup()
{
    ws2812_init();

    for( uint i = 0; i < WS2812_AMT; i++ )
    {
        ws2812_pixels[i].r = 0;
        ws2812_pixels[i].g = 0;
        ws2812_pixels[i].b = 0;
    }
    ws2812_update(true);
}


void ws2812_task()
{
    for( uint i = 0; i < WS2812_AMT; i++ )
    {
        ws2812_pixels[i].r = *WS_2812[i].value * 255 / WS_2812[i].limit;
        ws2812_pixels[i].g = *WS_2812[i].value * 255 / WS_2812[i].limit;
        ws2812_pixels[i].b = *WS_2812[i].value * 255 / WS_2812[i].limit;
    }
    ws2812_update(false);
}


