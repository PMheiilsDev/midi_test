#ifndef _MIDI_HANDLER_H
#define _MIDI_HANDLER_H


#include "tusb.h"

extern char* buffer;
extern uint64_t buffer_counter;



void process_midi_message(void);




#endif
