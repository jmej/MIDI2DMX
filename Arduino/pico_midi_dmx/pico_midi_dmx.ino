/*********************************************************************
 requires https://github.com/earlephilhower/arduino-pico (LGPL)
 and https://github.com/jostlowe/Pico-DMX (BSD-3)
 
 Simplified demo of usb midi in and out of a raspberry pi pico (tested on an RP2040)

 
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 Copyright (c) 2019 Ha Thach for Adafruit Industries
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

/* This sketch is enumerated as USB MIDI device. 
 * Following library is required
 * - MIDI Library by Forty Seven Effects
 *   https://github.com/FortySevenEffects/arduino_midi_library
 */

#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

const unsigned int colors[12][3]={
 {254, 0, 0}, //red
 {254, 127, 0}, //orange
 {254, 254, 0}, //yellow
 {127, 254, 0}, //lightgreen
 {0, 254, 0}, //green
 {0, 254, 127}, //cyan
 {0, 254, 254}, //lightblue
 {0, 127, 254}, //blue
 {0, 0, 254}, //darkblue
 {127, 0, 254}, //purple
 {254, 0, 254}, //pink
 {254, 0, 127}, //magenta
};

// #define RED    0xFF0000
// #define ORANGE 0xFF8000
// #define YELLOW 0xFFFF00
// #define LIGHTGREEN  0x80FF00
// #define GREEN  0x00FF00
// #define CYAN  0x00FF80
// #define LIGHTBLUE   0x00FFFF
// #define BLUE   0x0080FF
// #define DARKBLUE   0x0000FF
// #define PURPLE 0x8000FF
// #define PINK   0xFF00FF
// #define MAGENTA 0xFF0080

int val = 0;
int sensorPin = 26;

long counter = 0;
int vel = 64;
int hue = 0;
bool sustain = 0;

 uint universe_length;
 uint8_t universe[universe_length + 1]; 
// USB MIDI object
Adafruit_USBD_MIDI usb_midi;
DmxOutput myDmxOutput;

// Create a new instance of the Arduino MIDI Library,
// and attach usb_midi as the transport.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);



void setup() {
  // Manual begin() is required on core without built-in support e.g. mbed rp2040
  if (!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }

  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  usb_midi.setStringDescriptor("TinyUSB MIDI");

  // Initialize MIDI, and listen to all MIDI channels
  // This will also call usb_midi's begin()
  MIDI.begin(MIDI_CHANNEL_OMNI);
  myDmxOutput.begin(1);
  //test purple on a light setup with channel 1: red, channel 2: green, channel 3: blue
  universe[1] = 255;
  universe[2] = 0;
  universe[3] = 255;
  // If already enumerated, additional class driverr begin() e.g msc, hid, midi won't take effect until re-enumeration
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  // Attach the handleNoteOn function to the MIDI Library. It will
  // be called whenever the Bluefruit receives MIDI Note On messages.
  MIDI.setHandleNoteOn(handleNoteOn);

  // Do the same for MIDI Note Off messages.
  MIDI.setHandleNoteOff(handleNoteOff);
}

void loop() {
  #ifdef TINYUSB_NEED_POLLING_TASK
  // Manual call tud_task since it isn't called by Core's background
  TinyUSBDevice.task();
  #endif

  // not enumerated()/mounted() yet: nothing to do
  if (!TinyUSBDevice.mounted()) {
    return;
  }

  if(sustain){
    universe[1] = colors[hue][0];
    universe[2] = colors[hue][0];
    universe[3] = colors[hue][0];
    universe[7] = vel;
  }else{
    universe[7] = vel;
  }
  myDmxOutput.write(universe, universe_length + 1);
  usbMIDI.read(); // USB MIDI receive
  MIDI.read();

  
  val = analogRead(sensorPin);
  val = constrain(val, 100, 750);
  val = map(val, 100, 750, 0, 127);
  //Serial.println(val);
  MIDI.sendControlChange(7, val, 1);




  // read any new MIDI messages
  MIDI.read();
}

void handleNoteOn(byte channel, byte pitch, byte velocity) {
  hue = note % 12;
  vel = velocity;
  sustain = 1;
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
  vel = velocity;
  sustain = 0;
  // Log when a note is released.
  Serial.print("Note off: channel = ");
  Serial.print(channel);

  Serial.print(" pitch = ");
  Serial.print(pitch);

  Serial.print(" velocity = ");
  Serial.println(velocity);
  digitalWrite(LED_BUILTIN, LOW);
}
