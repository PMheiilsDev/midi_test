#ifndef _MIDI_HANDLER_H
#define _MIDI_HANDLER_H


#include "tusb.h"

extern char* buffer;
extern uint64_t buffer_counter;

typedef struct
{
    // 4 bytes long 
    // AAAA BBBB 0CCC CCCC 0DDD DDDD 
    // A => type of data ( eg control change note on/off ) 
    // B => Channel number 
        // C => which note or control it is 
        // D => velocity or value of control 

    // as voice meeter can use control change 
    // and note on/off for the sliders
    // there can be a maximum of 2 * (1<<4) * (1<<7) = 4096 


}
midi_measage_t

typedef struct
{
    char changeable : 1;
    char cable_num : 4;

    uint8_t value;
    uint8_t measage_type;
    uint8_t note;

}
midi_handler_t;


void process_midi_message(void);




#endif



// notes:
// to make it possible for the midi_handler to change the values of the inputs 
// create a struct that holds the pointer to the values of the intputs 

