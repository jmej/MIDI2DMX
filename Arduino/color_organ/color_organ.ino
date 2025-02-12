#include <MIDI.h>

extern void leds_update();
extern void led_color(unsigned int num, unsigned int rgb);
extern void led_color(unsigned int num, unsigned char red, unsigned char green, unsigned char blue);
extern void led_channel(unsigned int num, unsigned int val);
#define OFF    0x000000
#define WHITE  0xFFFFFF

const unsigned long colors[12]={
  0xFF0000,0xFF8000,0xFFFF00,0x80FF00,0x00FF00,0x00FF80,
  0x00FFFF,0x0080FF, 0x0000FF,0x8000FF,0xFF00FF,0xFF0080
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


MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

long counter = 0;
int vel = 64;
int hue = 0;
bool sustain = 0;

void setup() {
  Serial1.begin(250000);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

elapsedMillis timeout;

void OnControlChange(byte channel, byte control, byte value) {  
//  Serial.println("got a midi cc");  
    // int dmxChannel = (channel -1)*128 + control;  
    // led_channel(dmxChannel, value*2); 
//  Serial.println("got midi"); 
//  Serial.println(value);  
}

//todo = set channels based on polyphony so multiple lights could be on different channels
void OnNoteOn(byte channel, byte note, byte velocity) {
  hue = note % 12;
  vel = velocity;
  sustain = 1;
  Serial.println("got a midi note");
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  vel = velocity;
  sustain = 0;
  Serial.println("got a midi note off");
}

void loop() {
  if(sustain){
    led_color(1, colors[hue]);
    led_channel(7, vel);
  }else{
    led_channel(7, 0);
  }
  leds_update();
  usbMIDI.read(); // USB MIDI receive
  MIDI.read();
  delay(5);
}
