
#include "main_irq_callback.h"

void main_interupt_callback(uint gpio, uint32_t events)
{

    if ( gpio == IO_EXPANDER_INT_PIN )
    {
        IO_expander_interupt_callback( gpio, events );
    }
    else
    {
        sw_interupt_callback( gpio, events);
    }
}
