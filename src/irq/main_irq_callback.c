
#include "main_irq_callback.h"

void main_interupt_callback(uint gpio, uint32_t events)
{
    IO_expander_interupt_callback( gpio, events );
 
    sw_interupt_callback( gpio, events);
    
}
