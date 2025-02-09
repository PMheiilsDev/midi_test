import mido

print("Available MIDI Input Ports:")
devices = mido.get_input_names()
print(devices)
index = int(input())
midi_port_name = devices[index]

with mido.open_input(midi_port_name) as inport:
    for msg in inport:
        print(f"Received: {msg}")
