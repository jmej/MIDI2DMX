#include <MIDI.h>

extern void leds_update();
extern void led_color(unsigned int num, unsigned int rgb);
extern void led_color(unsigned int num, unsigned char red, unsigned char green, unsigned char blue);
extern void led_channel(unsigned int num, unsigned int val);
#define OFF    0x000000
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF

elapsedMillis timeout;

//these may need to change depending on your lights - only used for noteon, and assumes all lights start on channel 1 and respond together
int redChannel = 1;
int greenChannel = 2;
int blueChannel = 3;
int brightnessChannel = 7;

int scriabin[12][3] = {
  {255, 0, 0}, //red
  {255, 165, 0}, //orange
  {255, 255, 0}, //yellow
  {64,255,0}. //green
  {0, 255, 255} //turqoise
  {0, 191, 255}, //light blue
  {0, 0, 255}, //dark blue
  {128, 0, 255}. //purple
  {191, 0, 255}, //lavendar
  {255, 0 ,255}, //fuschia
  {255, 0, 128}, //pink
  {255, 0 , 64} //rose
}


MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

void setup() {
  Serial1.begin(250000);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(OnControlChange);
  MIDI.setHandleNoteOn(OnNoteOn);
  MIDI.setHandleNoteOff(OnNoteOff);
}


void loop() {
  leds_update();
  usbMIDI.read(); // USB MIDI receive
  MIDI.read();
}

void OnControlChange(byte channel, byte control, byte value) {
  led_channel(control, value*2);
//  Serial.println("got midi");
//  Serial.println(value);
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  led_channel(redChannel, scriabin[note%12][0]);
  led_channel(greenChannel, scriabin[note%12][1]);
  led_channel(blueChannel, scriabin[note%12][2]);
  led_channel(brightnessChannel, velocity*2);

}

void OnNoteOff(byte channel, byte note, byte velocity) {
  led_channel(brightnessChannel, 0);
}






