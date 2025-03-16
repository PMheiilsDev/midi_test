
# include "ws2812.h"

#include "pico/time.h"
#include "pico/stdio.h"
#include <stdio.h>

PIO ws2812_pio;
uint sm;
uint offset;

ws2812_pixel_t ws2812_pixels[WS2812_AMT];

uint64_t ws2812_last_update = 0;

void ws2812_init()
{
    pio_claim_free_sm_and_add_program( &ws2812_program, &ws2812_pio, &sm, &offset );

    ws2812_program_init( ws2812_pio, sm, offset, WS2812_PIN, 800000, false );

}

static void ws2812_set_pixel( uint8_t r, uint8_t g, uint8_t b )
{
    uint32_t rgb = ( g << 3*8 ) | ( r << 2*8 ) | ( b << 1*8 );
    pio_sm_put_blocking( ws2812_pio, sm, rgb );
}


void ws2812_update()
{
    int64_t time_since_last_update = time_us_64() - ws2812_last_update;
    if ( time_since_last_update < WS2812_T_REST )
    {
        sleep_us( WS2812_T_REST - time_since_last_update );
    }

    for ( uint i = 0; i < WS2812_AMT; i++ )
    {
        ws2812_set_pixel( ws2812_pixels[i].r, ws2812_pixels[i].g, ws2812_pixels[i].b );
    }
    ws2812_last_update = time_us_64();
}

