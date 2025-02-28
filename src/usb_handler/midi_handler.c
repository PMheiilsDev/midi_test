
#include "midi_handler.h"

char* buffer;
uint64_t buffer_counter = 0;



void process_midi_message(void) 
{
    uint8_t packet[4]; // USB MIDI messages are 4 bytes
    if (tud_midi_packet_read(packet)) 
    {
        uint8_t cable_number = packet[0] >> 4; // First 4 bits for cable number
        uint8_t status = packet[1];           // MIDI status byte
        uint8_t data1 = packet[2];            // First data byte (note, controller, etc.)
        uint8_t data2 = packet[3];            // Second data byte (velocity, value, etc.)

        uint8_t command = status & 0xF0;      // Extract command type
        uint8_t channel = (status & 0x0F) + 1; // MIDI channel (1-16)

        switch (command) 
        {
            case 0x80: // Note Off
                sprintf( buffer+128*buffer_counter, "Note Off: Note = %d, Velocity = %d, Channel = %d\n", data1, data2, channel);
            break;

            case 0x90: // Note On (check for velocity > 0)
                sprintf( buffer+128*buffer_counter, "Note On: Note = %d, Velocity = %d, Channel = %d\n", data1, data2, channel);
            break;

            case 0xA0: // Aftertouch (Polyphonic Pressure)
                sprintf( buffer+128*buffer_counter,  "Poly Aftertouch: Note = %d, Pressure = %d, Channel = %d\n", data1, data2, channel);
            break;

            case 0xB0: // Control Change
                sprintf( buffer+128*buffer_counter, "Control Change: Controller = %d, Value = %d, Channel = %d\n", data1, data2, channel);
            break;

            case 0xC0: // Program Change (Only 1 data byte)
                sprintf( buffer+128*buffer_counter, "Program Change: Program = %d, Channel = %d\n", data1, channel);
            break;

            case 0xD0: // Channel Aftertouch (Channel Pressure)
                sprintf( buffer+128*buffer_counter, "Channel Aftertouch: Pressure = %d, Channel = %d\n", data1, channel);
            break;

            case 0xE0: // Pitch Bend Change
            {
                int16_t bend_value = ((data2 << 7) | data1) - 8192; // 14-bit signed
                sprintf( buffer+128*buffer_counter, "Pitch Bend: Value = %d, Channel = %d\n", bend_value, channel);
            }
            break;

            default:
                sprintf( buffer+128*buffer_counter, "Unknown MIDI Message: %02X %02X %02X\n", status, data1, data2);
            break;
        }
    
        buffer_counter++;
    }
}


