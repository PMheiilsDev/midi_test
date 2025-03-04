
# include "ws2812_handler.h"

#include "pico/time.h"

PIO ws2812_pio;
uint sm;
uint offset;


void ws2812_init()
{
    
    pio_claim_free_sm_and_add_program( &ws2812_program, &ws2812_pio, &sm, &offset );

    ws2812_program_init( ws2812_pio, sm, offset, WS2812_PIN, 800000, false );

    for ( uint ji = 5; ji > 0; ji--)
    {
        for ( uint j = 200; j > 0; j-- )
        {
            for ( uint i = 0; i < 1000; i++ )
            {
                if ( j == i ) 
                    ws2812_set_pixel( 0, 50, 0 );

                else 
                    ws2812_set_pixel( 0, 0, 10 );
            }
            sleep_us( ji * 200 );
        }
        for ( uint j = 0; j < 200; j++ )
        {
            for ( uint i = 0; i < 1000; i++ )
            {
                if ( j == i ) 
                    ws2812_set_pixel( 0, 10, 0 );

                else 
                    ws2812_set_pixel( 0, 0, 10 );
            }
            sleep_us( ji * 200 );
        }
    }
    
    sleep_ms( 100 );
    for ( uint i = 0; i < 1000; i++ )
    {
        ws2812_set_pixel( 0, 0, 0 );
    }
}

void ws2812_set_pixel( uint8_t r, uint8_t g, uint8_t b )
{
    uint32_t rgb = ( g << 3*8 ) | ( r << 2*8 ) | ( b << 1*8 );
    pio_sm_put_blocking( ws2812_pio, sm, rgb );
}

