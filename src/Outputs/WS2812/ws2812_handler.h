#ifndef _WS2812_HANDLER_H
#define _WS2812_HANDLER_H

#include "ws2812.pio.h"

#define WS2812_PIN 7 

#define WS2812_PIO pio0 

void ws2812_init(); 

void ws2812_set_pixel( uint8_t r, uint8_t g, uint8_t b );


#endif
