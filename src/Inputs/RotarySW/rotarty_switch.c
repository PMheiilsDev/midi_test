

#include "rotarty_switch.h"




rotary_switch_t rotary_switches[MAX_ROTARY_SWITCHES] = 
{
    {
        .clk_pin =  2,
        .data_pin =  3,
        .note = 82,
        .counter = 0,
        .last_sent = 0
    },
    {
        .clk_pin =  4,
        .data_pin =  5,
        .note = 83,
        .counter = 0,
        .last_sent = 0
    },
    {
        .clk_pin =  10,
        .data_pin =  11,
        .note = 84,
        .counter = 0,
        .last_sent = 0
    },
    {
        .clk_pin =  14,
        .data_pin =  15,
        .note = 85,
        .counter = 0,
        .last_sent = 0
    }
};


void rot_sw_setup(void)
{
    for (int i = 0; i < MAX_ROTARY_SWITCHES; i++) 
    {
        gpio_init(rotary_switches[i].clk_pin);
        gpio_set_dir(rotary_switches[i].clk_pin, GPIO_IN);
        gpio_pull_up(rotary_switches[i].clk_pin);

        gpio_init(rotary_switches[i].data_pin);
        gpio_set_dir(rotary_switches[i].data_pin, GPIO_IN);
        gpio_pull_up(rotary_switches[i].data_pin);

        gpio_set_irq_enabled_with_callback(rotary_switches[i].clk_pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &sw_interupt_callback);
        gpio_set_irq_enabled_with_callback(rotary_switches[i].data_pin, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &sw_interupt_callback);
    }
}


void update_counter(rotary_switch_t *rot_sw, bool up)
{
    if (up) 
    {
        if (rot_sw->counter < 127) 
        {
            rot_sw->counter += 1;
        }
    } 
    else 
    {
        if (rot_sw->counter > 0) 
        {
            rot_sw->counter -= 1;
        }
    }
}


void sw_interupt_callback(uint gpio, uint32_t events)
{
    for (int i = 0; i < MAX_ROTARY_SWITCHES; i++) 
    {
        rotary_switch_t *rot_sw = &rotary_switches[i];
        if (gpio == rot_sw->clk_pin || gpio == rot_sw->data_pin) {
            bool clk = gpio_get(rot_sw->clk_pin);
            bool data = gpio_get(rot_sw->data_pin);

            if (events & GPIO_IRQ_EDGE_RISE) 
            {
                if (gpio == rot_sw->clk_pin) 
                {
                    update_counter(rot_sw, data == 0);
                } 
                else if (gpio == rot_sw->data_pin) 
                {
                    update_counter(rot_sw, clk == 1);
                }
            } 
            else if (events & GPIO_IRQ_EDGE_FALL) 
            {
                if (gpio == rot_sw->clk_pin) {
                    update_counter(rot_sw, data == 1);
                } 
                else if (gpio == rot_sw->data_pin) 
                {
                    update_counter(rot_sw, clk == 0);
                }
            }
        }
    }
}


void rot_sw_task(void)
{
    for (int i = 0; i < MAX_ROTARY_SWITCHES; i++) 
    {
        rotary_switch_t *rot_sw = &rotary_switches[i];
        if (rot_sw->last_sent != rot_sw->counter) 
        {
            rot_sw->last_sent = rot_sw->counter;
            uint8_t note_on[3] = {0b10110000 | 0, rot_sw->note, 0x7F & rot_sw->counter};
            tud_midi_stream_write(0, note_on, 3);
        }
    }
}

