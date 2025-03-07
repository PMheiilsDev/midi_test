#ifndef _MIDI_HANDLER_H
#define _MIDI_HANDLER_H


#include "tusb.h"

extern char* buffer;
extern uint64_t buffer_counter;



void process_midi_message(void);




#endif



// notes:
// to make it possible for the midi_handler to change the values of the inputs 
// create a struct that holds the pointer to the values of the intputs 

