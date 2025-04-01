#include <MIDI.h>

extern void leds_update();
extern void led_color(unsigned int num, unsigned int rgb);
extern void led_color(unsigned int num, unsigned char red, unsigned char green, unsigned char blue);
extern void led_channel(unsigned int num, unsigned int val);
#define OFF    0x000000
#define WHITE  0xFFFFFF

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

int midiChannel = 1;
int dmxStart = 0;
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);
#define universe_length 512
uint8_t universe[universe_length + 1]; 
long counter = 0;
int vel = 64;
int hue = 0;
bool sustain = 0;

void setup() {
  Serial1.begin(250000);
  usbMIDI.setHandleControlChange(OnControlChange);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandleNoteOff(OnNoteOff);
  MIDI.begin();
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
  midiChannel = channel;
  hue = note % 12;
  vel = velocity;
  sustain = 1;
  Serial.println("got a midi note");
  Serial.print("ch: ");
  Serial.println(channel);
}

void OnNoteOff(byte channel, byte note, byte velocity) {
  midiChannel = channel;
  vel = velocity;
  sustain = 0;
  //Serial.println("got a midi note off");
}

void loop() {
  dmxStart = (midiChannel * 10) - 10;
  if(sustain){
      led_channel(1+dmxStart, colors[hue][0]);
      led_channel(2+dmxStart, colors[hue][1]);
      led_channel(3+dmxStart, colors[hue][2]);
      led_channel(7+dmxStart, vel);
    }else{
      led_channel(7+dmxStart, 0);
    }
  // Serial.print("dmxStart: ");
  // Serial.println(dmxStart);
  leds_update();
  usbMIDI.read(); // USB MIDI receive
  MIDI.read();
  delay(1);
}
