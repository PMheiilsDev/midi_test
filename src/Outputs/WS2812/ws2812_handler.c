
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

typedef struct {
    int r;
    int g;
    int b;
} RGB;


// Convert HSV (Hue, Saturation, Value) to RGB
RGB hsvToRgb(float h, float s, float v) 
{
    float c = v * s;
    float x = c * (1 - ((int)(h / 60) % 2 ? h / 60 - (int)(h / 60) : 1 - (h / 60 - (int)(h / 60))));
    float m = v - c;
    float r, g, b;

    if (h < 60) 
    {
        r = c; g = x; b = 0;
    } 
    else if (h < 120) 
    {
        r = x; g = c; b = 0;
    } 
    else if (h < 180) 
    {
        r = 0; g = c; b = x;
    } 
    else if (h < 240) 
    {
        r = 0; g = x; b = c;
    } 
    else if (h < 300) 
    {
        r = x; g = 0; b = c;
    } 
    else 
    {
        r = c; g = 0; b = x;
    }

    RGB color = 
    {
        (int)((r + m) * 255),
        (int)((g + m) * 255),
        (int)((b + m) * 255)
    };

    return color;
}

RGB indexToCustomRainbow(int index) 
{
    float hue = 240.0 - ((float)index / 256.0) * (240.0 - 0.0); 
    return hsvToRgb(hue, 1.0, 1.0);  
}

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
        RGB rgb = indexToCustomRainbow(*WS_2812[i].value * 255 / WS_2812[i].limit);

        ws2812_pixels[i].r = rgb.r>>4;
        ws2812_pixels[i].g = rgb.g>>4;
        ws2812_pixels[i].b = rgb.b>>4;
    }
    ws2812_update(false);
}


