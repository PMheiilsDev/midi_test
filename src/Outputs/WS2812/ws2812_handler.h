#ifndef _WS2812_HANDLER_H
#define _WS2812_HANDLER_H

#include "ws2812.h"

#include "adc_handler.h"
#include "rotarty_switch.h"

typedef struct 
{
    const uint8_t limit;
    volatile uint8_t* value;
}
WS_2812_t;

extern WS_2812_t WS_2812[WS2812_AMT];

void ws2812_setup();

void ws2812_task();


#endif
