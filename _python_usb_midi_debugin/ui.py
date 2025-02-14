import sys
import mido
from PyQt6.QtWidgets import (
    QApplication, QWidget, QPushButton, QGridLayout, QVBoxLayout,
    QSlider, QHBoxLayout, QLabel, QFrame, QComboBox, QTextEdit
)
from PyQt6.QtCore import Qt, QThread, pyqtSignal


class MidiListenerThread(QThread):
    midi_message_received = pyqtSignal(str)

    def __init__(self, input_device_name):
        super().__init__()
        self.input_device_name = input_device_name
        self.running = True

    def run(self):
        try:
            with mido.open_input(self.input_device_name) as inport:
                while self.running:
                    for msg in inport.iter_pending():
                        message = f"Received MIDI: {msg}"
                        self.midi_message_received.emit(message)
        except Exception as e:
            self.midi_message_received.emit(f"Error: {e}")

    def stop(self):
        self.running = False
        self.quit()


class GridWindow(QWidget):
    def __init__(self):
        super().__init__()

        self.checkable_buttons = [True, False, False, False,
                                  False, True, False, False,
                                  False, False, True, False,
                                  False, False, False, True]

        self.outport = None
        self.listener_thread = None

        self.init_ui()

    def init_ui(self):
        self.setWindowTitle("4x4 Grid of Buttons with Sliders")
        self.resize(800, 600)  # **Increase startup window size**

        main_layout = QVBoxLayout()

        # MIDI Output Device Selection
        output_device_layout = QHBoxLayout()
        self.device_combo_output = QComboBox()
        self.device_combo_output.addItems(mido.get_output_names())
        self.device_combo_output.currentIndexChanged.connect(self.update_midi_output_device)
        output_device_layout.addWidget(self.device_combo_output)

        refresh_button_output = QPushButton("Refresh")
        refresh_button_output.clicked.connect(self.refresh_output_devices)
        refresh_button_output.setFixedSize(100, 30)  # **Set fixed size for the refresh button**
        output_device_layout.addWidget(refresh_button_output)

        main_layout.addLayout(output_device_layout)

        # 4x4 Grid of Buttons
        grid = QGridLayout()
        self.buttons = []
        num = 1
        for row in range(4):
            for col in range(4):
                button = QPushButton(f"{num}")
                button.setCheckable(self.checkable_buttons[num - 1])
                button.clicked.connect(self.create_button_callback(button, num))
                grid.addWidget(button, row, col)
                self.buttons.append(button)
                num += 1

        grid.setHorizontalSpacing(10)
        grid.setVerticalSpacing(10)
        grid.setAlignment(Qt.AlignmentFlag.AlignLeft)
        main_layout.addLayout(grid)

        # Divider
        divider = QFrame()
        divider.setFrameShape(QFrame.Shape.HLine)
        divider.setFrameShadow(QFrame.Shadow.Sunken)
        main_layout.addWidget(divider)

        # Sliders and Reset Buttons
        for idx in range(1, 3):
            slider_layout = QHBoxLayout()

            send_button = QPushButton("Send")
            send_button.clicked.connect(lambda _, i=idx: self.send_slider_value(i))
            send_button.setFixedSize(60, 30)
            slider_layout.addWidget(send_button)

            slider = QSlider(Qt.Orientation.Horizontal)
            slider.setRange(0, 127)
            slider.valueChanged.connect(lambda _, i=idx: self.update_slider_value(i))
            slider_layout.addWidget(slider)

            slider_value_label = QLabel(f"Value: {slider.value()}")
            slider_value_label.setFixedWidth(80)
            slider_layout.addWidget(slider_value_label)

            reset_button = QPushButton("R")
            reset_button.setFixedSize(30, 30)
            reset_button.clicked.connect(lambda _, i=idx: self.reset_slider_and_send(i))
            slider_layout.addWidget(reset_button)

            setattr(self, f"slider{idx}", slider)
            setattr(self, f"slider{idx}_value_label", slider_value_label)

            main_layout.addLayout(slider_layout)

        # Divider
        bottom_divider = QFrame()
        bottom_divider.setFrameShape(QFrame.Shape.HLine)
        bottom_divider.setFrameShadow(QFrame.Shadow.Sunken)
        main_layout.addWidget(bottom_divider)

        # MIDI Input Device Selection
        input_device_layout = QHBoxLayout()
        self.device_combo_input = QComboBox()
        self.device_combo_input.addItems(mido.get_input_names())
        self.device_combo_input.currentIndexChanged.connect(self.update_midi_input_device)
        input_device_layout.addWidget(self.device_combo_input)

        refresh_button_input = QPushButton("Refresh")
        refresh_button_input.clicked.connect(self.refresh_input_devices)
        refresh_button_input.setFixedSize(100, 30)  # **Set fixed size for the refresh button**
        input_device_layout.addWidget(refresh_button_input)

        main_layout.addLayout(input_device_layout)

        # Log Display
        self.log_display = QTextEdit()
        self.log_display.setReadOnly(True)
        self.log_display.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap)  # **Ensure messages stay in one line**
        main_layout.addWidget(self.log_display)

        self.setLayout(main_layout)
        self.refresh_input_devices()
        self.refresh_output_devices()
        self.show()

    #region MIDI Functions

    def refresh_input_devices(self):
        # Temporarily disconnect the signal to prevent triggering
        self.device_combo_input.currentIndexChanged.disconnect()

        # Save the currently selected input device (if any)
        current_device = self.device_combo_input.currentText()

        # Clear and refresh the device list
        self.device_combo_input.clear()
        devices = mido.get_input_names()
        
        # Prioritize TinyUSB devices
        tinyusb_devices = [device for device in devices if "TinyUSB" in device]
        other_devices = [device for device in devices if "TinyUSB" not in device]
        
        # Sort both lists
        tinyusb_devices.sort()
        other_devices.sort()
        
        # Combine the lists with TinyUSB devices first
        sorted_devices = tinyusb_devices + other_devices
        self.device_combo_input.addItems(sorted_devices)

        # Reconnect the signal after refresh
        self.device_combo_input.currentIndexChanged.connect(self.update_midi_input_device)

        # If the same device still exists, reselect it
        if current_device in sorted_devices:
            index = sorted_devices.index(current_device)
            self.device_combo_input.setCurrentIndex(index)
        else:
            # No device selected after refresh
            self.device_combo_input.setCurrentIndex(-1)

        # Manually trigger the update in case of reselecting the same item
        self.update_midi_input_device()

    def refresh_output_devices(self):
        # Temporarily disconnect the signal to prevent triggering
        self.device_combo_output.currentIndexChanged.disconnect()

        # Save the currently selected output device (if any)
        current_device = self.device_combo_output.currentText()

        # Clear and refresh the device list
        self.device_combo_output.clear()
        devices = mido.get_output_names()
        
        # Prioritize TinyUSB devices
        tinyusb_devices = [device for device in devices if "TinyUSB" in device]
        other_devices = [device for device in devices if "TinyUSB" not in device]
        
        # Sort both lists
        tinyusb_devices.sort()
        other_devices.sort()
        
        # Combine the lists with TinyUSB devices first
        sorted_devices = tinyusb_devices + other_devices
        self.device_combo_output.addItems(sorted_devices)

        # Reconnect the signal after refresh
        self.device_combo_output.currentIndexChanged.connect(self.update_midi_output_device)

        # If the same device still exists, reselect it
        if current_device in sorted_devices:
            index = sorted_devices.index(current_device)
            self.device_combo_output.setCurrentIndex(index)
        else:
            # No device selected after refresh
            self.device_combo_output.setCurrentIndex(-1)

        # Manually trigger the update in case of reselecting the same item
        self.update_midi_output_device()


    def update_midi_input_device(self):
        device_name = self.device_combo_input.currentText()
        if device_name:
            if self.listener_thread:
                self.listener_thread.stop()
                self.listener_thread.wait()

            self.listener_thread = MidiListenerThread(device_name)
            self.listener_thread.midi_message_received.connect(self.log_midi_message)
            self.listener_thread.start()
            self.log_midi_message(f"MIDI input device selected: {device_name}")
        else:
            self.log_midi_message("No MIDI input device selected.")

    def log_midi_message(self, message):
        self.log_display.append(message)

    def update_midi_output_device(self):
        device_name = self.device_combo_output.currentText()
        if device_name:
            try:
                self.outport = mido.open_output(device_name)
                self.log_midi_message(f"MIDI output device selected: {device_name}")
            except Exception as e:
                self.log_midi_message(f"Error opening MIDI output device: {e}")
        else:
            self.log_midi_message("No MIDI output device selected.")

    #endregion

    #region Button Functions

    def create_button_callback(self, button, button_num):
        def callback():
            self.button_clicked(button, button_num, button.isChecked())
        return callback

    def button_clicked(self, button, button_num, status):
        if self.outport:
            velocity = 127 if status else 0
            msg = mido.Message('note_on', note=button_num, velocity=velocity)
            self.outport.send(msg)
            self.log_midi_message(f"Button {button_num} clicked, Status: {status}, MIDI sent: {msg}")
        else:
            self.log_midi_message("No MIDI output device selected!")

    #endregion

    #region Slider Functions

    def reset_slider_and_send(self, slider_idx):
        slider = getattr(self, f"slider{slider_idx}")
        slider.setValue(slider.minimum())
        self.send_slider_value(slider_idx)

    def send_slider_value(self, slider_idx):
        if self.outport:
            slider = getattr(self, f"slider{slider_idx}")
            velocity = slider.value()
            note = 100 if slider_idx == 1 else 101
            msg = mido.Message('note_on', note=note, velocity=velocity)
            self.outport.send(msg)
            self.log_midi_message(f"Slider {slider_idx} sent MIDI message: {msg}")
        else:
            self.log_midi_message("No MIDI output device selected!")

    def update_slider_value(self, slider_idx):
        slider = getattr(self, f"slider{slider_idx}")
        label = getattr(self, f"slider{slider_idx}_value_label")
        label.setText(f"Value: {slider.value()}")

    #endregion


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = GridWindow()
    sys.exit(app.exec())
