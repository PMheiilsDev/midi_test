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

void adc_setup(void);

void adc_task(void);

static inline int gpio_to_adc_channel(uint gpio);

#endif


// ADC ideas
// create a reapeating timer and read the adc value every so often
//
// create a function that can simulate a Highpass filter by interpolating 
// filter values that are off from average 
//  - this might make add delay


