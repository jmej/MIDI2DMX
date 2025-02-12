#include <MIDI.h>

extern void leds_update();
extern void led_color(unsigned int num, unsigned int rgb);
extern void led_color(unsigned int num, unsigned char red, unsigned char green, unsigned char blue);
extern void led_channel(unsigned int num, unsigned int val);
#define OFF    0x000000
#define RED    0xFF0000
#define ORANGE 0xFF8000
#define YELLOW 0xFFFF00
#define LIGHTGREEN  0x80FF00
#define GREEN  0x00FF00
#define CYAN  0x00FF80
#define BLUE   0x0000FF

#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF

// c: 255, 0, 0 #ff0000 red
// c#: 255, 128, 0 #ff8000 orange
// d: 255, 255, 0 #ffff00 yellow
// d#: 128, 255, 0 #80ff00 light green
// e: 0, 255, 0 #00ff00 green
// f: 0, 255, 128 #00ff80 cyan
// f# 0, 255, 255 #00ffff light blue
// g: 0, 128, 255 #0080ff blue
// g# 0, 0, 255 #0000ff dark blue
// a: 128, 0, 255 #8000ff purple
// a# 255, 0, 255 #ff00ff pink
// b: 255, 0, 128 #ff0080 magenta

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

long counter = 0;
int decay = 64; //starts decay at the halfway point
//maybe map functions will look something like 0, 127, 10, 5000?


void setup() {
  Serial1.begin(250000);
  usbMIDI.setHandleControlChange(OnControlChange);
  //usbMIDI.setHandleNoteOn(OnNoteIn); //future stuff
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(OnControlChange);
}

elapsedMillis timeout;

void OnControlChange(byte channel, byte control, byte value) {  
//  Serial.println("got a midi cc");  
    int dmxChannel = (channel -1)*128 + control;  
    led_channel(dmxChannel, value*2); 
//  Serial.println("got midi"); 
//  Serial.println(value);  
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  Serial.println("got a midi note");
  
}

void loop() {
  leds_update();
  usbMIDI.read(); // USB MIDI receive
  MIDI.read();
  delay(5);
}
