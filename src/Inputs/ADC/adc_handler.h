#ifndef _ADC_HANDLER_H
#define _ADC_HANDLER_H

#include <inttypes.h>
#include <stdbool.h>

#include "hardware/gpio.h"
#include "hardware/adc.h"


#include "hardware/pwm.h"
#include "tusb.h"

#define MAX_ADC_CHANNELS 4

#define ADC_MUL_PLEX_GPIO_0 18
#define ADC_MUL_PLEX_GPIO_1 19 
#define ADC_MUL_PLEX_GPIO_2 20

typedef struct 
{
    uint8_t pin;
    uint8_t note;
    uint8_t is_mul_plex: 1, mul_plex_channel : 3; // assuming that the multiplexer has 3 pins for channel selection 
    uint8_t mul_plex[3];
    volatile uint8_t result;                   // change that to a uint8_t 
    volatile uint8_t result_pref;
    uint8_t num_reads;
} 
adc_channel_t;

extern adc_channel_t adc_channels[MAX_ADC_CHANNELS];

/// @brief setting up all necessary ADCs from cdc_channels 
void adc_setup(void);

/// @brief handling all ADCs from adc_channels 
void adc_task(void);

/// @brief converting a GPIO pin to an ADC channel 
/// @param gpio GPIO pin number 
/// @return ADC channel number 
static inline int gpio_to_adc_channel(uint gpio);

/// @brief set the value of the gpio pins set in the adc_channel
/// @param adc_channel pointer to the adc_channel_t 
void adc_set_mul_plex_gpios( adc_channel_t *adc_channel );

#endif


// ADC ideas
// create a reapeating timer and read the adc value every so often
//
// create a function that can simulate a Highpass filter by interpolating 
// filter values that are off from average 
//  - this might make add delay


// ADC TODO 
// - add support for using analog multiplexer to read multiple analog inputs

// ((( it might be possible to check if a multiplexer is connected by connecting one of the 8 channels to a fixed level )))

