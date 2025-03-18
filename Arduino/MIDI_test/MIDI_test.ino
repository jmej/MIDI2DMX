#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void setup()
{
    // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
      // Manual begin() is required on core without built-in support e.g. mbed rp2040
    if (!TinyUSBDevice.isInitialized()) {
      TinyUSBDevice.begin(0);
    }
    Serial.begin(115200);
    // Initialize MIDI, and listen to all MIDI channels
    // This will also call usb_midi's begin()
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.setHandleNoteOn(handleNoteOn);
    // Do the same for MIDI Note Off messages.
    MIDI.setHandleNoteOff(handleNoteOff);
      // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
    if (TinyUSBDevice.mounted()) {
      TinyUSBDevice.detach();
      delay(10);
      TinyUSBDevice.attach();
    }
    
}

void loop() {
  #ifdef TINYUSB_NEED_POLLING_TASK
  // Manual call tud_task since it isn't called by Core's background
  TinyUSBDevice.task();
  #endif

  if (!TinyUSBDevice.mounted()) {
    return;
  }
  MIDI.read();
  delay(5);
  Serial.print(".");
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  Serial.println("got a midi note");
  // Log when a note is pressed.
  Serial.print("Note on: channel = ");
  Serial.print(channel);

  Serial.print(" pitch = ");
  Serial.print(pitch);

  Serial.print(" velocity = ");
  Serial.println(velocity);
  digitalWrite(LED_BUILTIN, HIGH);
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  // Log when a note is released.
  Serial.print("Note off: channel = ");
  Serial.print(channel);

  Serial.print(" pitch = ");
  Serial.print(pitch);

  Serial.print(" velocity = ");
  Serial.println(velocity);
  digitalWrite(LED_BUILTIN, LOW);
}
