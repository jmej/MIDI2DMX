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

int rNew = 0;
int gNew = 0;
int bNew = 0;

int rOld = 0;
int gOld = 0;
int bOld = 0;

int counter = 0;

int fadeTime = 1000;


void setup() {
  Serial1.begin(250000);
  usbMIDI.setHandleControlChange(OnControlChange);
}

elapsedMillis timeout;

void OnControlChange(byte channel, byte control, byte value) {
  led_channel(control, value*2);
//  Serial.println("got midi");
//  Serial.println(value);
}

void loop() {
  if (counter == fadeTime){
    counter = counter % fadeTime;
    rOld = rNew;
    gOld = gNew;
    bOld = bNew;
    rNew = random(255);
    gNew = random(255);
    bNew = random(255);
  }else{
    counter++;
  }

  int r = map(counter, 0, fadeTime, rOld, rNew);
  int g = map(counter, 0, fadeTime, gOld, gNew);
  int b = map(counter, 0, fadeTime, bOld, bNew);
  
  leds_update();
  led_channel(1, r);
  led_channel(2, g);
  led_channel(3, b);
  usbMIDI.read(); // USB MIDI receive
  delay(1);
}





