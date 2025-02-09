import mido

print("Available MIDI Output Ports:")
devices = mido.get_output_names()
print(devices)
index = int(input())
midi_port_name = devices[index]

with mido.open_output(midi_port_name) as outport:
    # Example: Send a MIDI Note On message (Middle C, velocity 64)
    msg = mido.Message('note_on', note=60, velocity=64)
    outport.send(msg)
    
    # Wait a bit and then send Note Off to stop the sound
    import time
    time.sleep(1)
    msg = mido.Message('note_off', note=60)
    outport.send(msg)

