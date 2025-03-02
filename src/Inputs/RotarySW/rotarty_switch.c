#include "rotarty_switch.h"


const uint8_t rot_sw_note = 82;

volatile uint8_t sw_ctr = 0;
volatile uint8_t sw_ctr_last_sent = 0;


void rot_sw_setup(void)
{
    gpio_init(7);
    gpio_set_dir(7, GPIO_OUT);

    gpio_init(ROT_SW_DATA_PIN);
    gpio_set_dir(ROT_SW_DATA_PIN, GPIO_IN );
    gpio_pull_up(ROT_SW_DATA_PIN);

    gpio_init(ROT_SW_CLK_PIN);
    gpio_set_dir(ROT_SW_CLK_PIN, GPIO_IN );
    gpio_pull_up(ROT_SW_CLK_PIN);

    gpio_set_irq_enabled_with_callback(ROT_SW_CLK_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &sw_interupt_callback);
    gpio_set_irq_enabled_with_callback(ROT_SW_DATA_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &sw_interupt_callback);
}

void update_counter(bool up)
{
    if (up)
    {
        if (sw_ctr < 127)
        {
            sw_ctr += 1;
        }
    }
    else
    {
        if (sw_ctr > 0)
        {
            sw_ctr -= 1;
        }
    }
}

void sw_interupt_callback(uint gpio, uint32_t events)
{
    bool clk = gpio_get(ROT_SW_CLK_PIN);
    bool data = gpio_get(ROT_SW_DATA_PIN);

    if (events & GPIO_IRQ_EDGE_RISE)
    {
        if (gpio == ROT_SW_CLK_PIN)
        {
            update_counter(data == 0);
        }
        else if (gpio == ROT_SW_DATA_PIN)
        {
            update_counter(clk == 1);
        }
    }
    else if (events & GPIO_IRQ_EDGE_FALL)
    {
        if (gpio == ROT_SW_CLK_PIN)
        {
            update_counter(data == 1);
        }
        else if (gpio == ROT_SW_DATA_PIN)
        {
            update_counter(clk == 0);
        }
    }
}

void rot_sw_task(void)
{
    if ( sw_ctr_last_sent != sw_ctr )
    {
        sw_ctr_last_sent = sw_ctr;
        uint8_t note_on[3] = { 0b10110000 | 0, rot_sw_note, 0x7F & sw_ctr };
        tud_midi_stream_write( 0, note_on, 3);
    }
}

