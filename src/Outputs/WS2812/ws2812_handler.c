
# include "ws2812_handler.h"

#include "pico/time.h"
#include "pico/stdio.h"
#include <stdio.h>

PIO ws2812_pio;
uint sm;
uint offset;


void ws2812_init()
{
    stdio_init_all();
    
    pio_claim_free_sm_and_add_program( &ws2812_program, &ws2812_pio, &sm, &offset );

    ws2812_program_init( ws2812_pio, sm, offset, WS2812_PIN, 800000, false );

    for ( uint i = 0; i < 100000 ; i++ )
    {
        sleep_us(WS2812_T_REST);
        ws2812_set_pixel( 1, 1, 1 );
    }
    sleep_ms(10);
    ws2812_set_pixel( 0, 0, 0 );
}

void ws2812_set_pixel( uint8_t r, uint8_t g, uint8_t b )
{
    uint32_t rgb = ( g << 3*8 ) | ( r << 2*8 ) | ( b << 1*8 );
    pio_sm_put_blocking( ws2812_pio, sm, rgb );
}

