#ifndef _ADC_HANDLER_H
#define _ADC_HANDLER_H

#include <inttypes.h>
#include <stdbool.h>

#include "hardware/gpio.h"
#include "hardware/adc.h"


#include "hardware/pwm.h"
#include "tusb.h"

#define MAX_ADC_CHANNELS 4
#define NUM_ADC_READS 20

typedef struct 
{
    uint8_t pin;
    uint8_t note;
    uint16_t result;
    uint16_t result_pref;
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

#endif


// ADC ideas
// create a reapeating timer and read the adc value every so often
//
// create a function that can simulate a Highpass filter by interpolating 
// filter values that are off from average 
//  - this might make add delay


