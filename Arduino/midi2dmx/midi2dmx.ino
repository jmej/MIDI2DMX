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

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

long counter = 0;
int decay = 64; //starts decay at the halfway point
//maybe map functions will look something like 0, 127, 10, 5000?


void setup() {
  Serial1.begin(250000);
  usbMIDI.setHandleControlChange(OnControlChange);
  //usbMIDI.setHandleNoteOn(OnNoteIn); //future stuffs
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleControlChange(OnControlChange);
}

elapsedMillis timeout;

void OnControlChange(byte channel, byte control, byte value) {  
//  Serial.println("got a midi cc");  
    int ch = (channel -1)*128 + control;  
    led_channel(ch, value*2); 
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

int msToModTime(int ms){ //function to convert ms values into modulo numbers based on framerate delay of 5ms
  
}
