
#include <inttypes.h>
#include <stdbool.h>


#define BUTTON_AMT 1

extern bool button_send_midi_active[BUTTON_AMT];

extern uint8_t button_midi_num[BUTTON_AMT];

extern bool button_is_push_button[BUTTON_AMT];

extern uint8_t button_gpio_num[BUTTON_AMT];


static inline void button_init();

bool button_get_state(uint8_t);
bool button_get_state_midi_number(uint8_t);
bool button_get_state_gpio(uint8_t);

static uint8_t button_get_gpio( uint8_t );
