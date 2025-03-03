#ifndef _ADC_HANDLER_H
#define _ADC_HANDLER_H

#include <inttypes.h>
#include <stdbool.h>

#include "hardware/gpio.h"
#include "hardware/adc.h"


#include "hardware/pwm.h"
#include "tusb.h"


#define ADC_PIN 28
#define PWM_PIN 11
#define LED_PIN 22

void adc_setup(void);

void adc_task(void);

static inline int gpio_to_adc_channel(uint gpio);


#endif
