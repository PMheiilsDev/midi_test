#ifndef _WS2812_H
#define _WS2812_H

#include "ws2812.pio.h"

#define WS2812_PIN 22

#define WS2812_AMT 8

// never got an error when using 110 us 
// some datasheets say 50 us (100 us gives errors), some say 280 us
#define WS2812_T_REST 500 

#define WS2812_PIO pio0 

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} 
ws2812_pixel_t;

extern ws2812_pixel_t ws2812_pixels[WS2812_AMT];

void ws2812_init(); 

//void ws2812_set_pixel( uint8_t r, uint8_t g, uint8_t b );

void ws2812_update(bool wait);

#endif
