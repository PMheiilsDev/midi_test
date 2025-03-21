
#include "IOexpander_handler.h"

IO_exp_midi_element_t IO_exp_midi_element[IO_EXPANDER_MIDI_ELEMENT_AMT] =
{
    {
        .input_pin = 0,
        .output_pin = 5,
        .note = 10,
        .state = 0,
        .state_pref = 0
    },
    {
        .input_pin = 1,
        .output_pin = 7,
        .note = 11,
        .state = 0,
        .state_pref = 0
    },
    {
        .input_pin = 2,
        .output_pin = 5,
        .note = 12,
        .state = 0,
        .state_pref = 0
    },
    {
        .input_pin = 3,
        .output_pin = 5,
        .note = 13,
        .state = 0,
        .state_pref = 0
    },
    {
        .input_pin = 8,
        .output_pin = 5,
        .note = 14,
        .state = 0,
        .state_pref = 0
    },
    {
        .input_pin = 10,
        .output_pin = 5,
        .note = 15,
        .state = 0,
        .state_pref = 0
    },
    {
        .input_pin = 12,
        .output_pin = 5,
        .note = 16,
        .state = 0,
        .state_pref = 0
    },
    {
        .input_pin = 15,
        .output_pin = 5,
        .note = 17,
        .state = 0,
        .state_pref = 0
    },
};


void IO_expander_handler_init()
{
    IOexpander_init();

    for( uint i = 0; i < IO_EXPANDER_MIDI_ELEMENT_AMT; i++ )
    {
        //IOexpander_set_function( IO_exp_midi_element[i].output_pin, IO_EXPANDER_PIN_FUNC_OUTPUT );
        //IOexpander_put( IO_exp_midi_element[i].output_pin, IO_EXPANDER_PIN_OUTPUT_STATE_HIGH_Z );

        IOexpander_set_function( IO_exp_midi_element[i].input_pin, IO_EXPANDER_PIN_FUNC_INPUT );
    }
}


void IO_expander_handler_handler()
{
    for( uint i = 0; i < IO_EXPANDER_MIDI_ELEMENT_AMT; i++ )
    {
        IO_exp_midi_element[i].state = IOexpander_get( IO_exp_midi_element[i].input_pin, false ); 

        if( IO_exp_midi_element[i].state != IO_exp_midi_element[i].state_pref ) 
        {
            IO_exp_midi_element[i].state_pref = IO_exp_midi_element[i].state; 
            uint8_t note_on[3] = {0b10110000 | 0, IO_exp_midi_element[i].note, IO_exp_midi_element[i].state*127};
            tud_midi_stream_write(0, note_on, 3);

            if ( IO_exp_midi_element[i].state )
            {
                //IOexpander_put(IO_exp_midi_element[i].output_pin, !IOexpander_get(IO_exp_midi_element[i].output_pin, false) );
            }
        }
    }
}


