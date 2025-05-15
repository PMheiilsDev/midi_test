#ifndef _IO_EXP_HANDLER_H
#define _IO_EXP_HANDLER_H

#include "IOexpander.h" 


#define IO_EXPANDER_MIDI_ELEMENT_AMT 8 
 
typedef struct 
{
    char state : 1,state_pref : 1;

    uint8_t input_pin: 4, output_pin: 4;

    uint8_t note;

    uint8_t value;
}
IO_exp_midi_element_t;

extern IO_exp_midi_element_t IO_exp_midi_element[IO_EXPANDER_MIDI_ELEMENT_AMT];

void IO_expander_handler_init(); 

void IO_expander_handler_handler(); 

#endif
