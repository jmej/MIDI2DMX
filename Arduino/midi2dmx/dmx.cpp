#include <Arduino.h>

extern void leds_update();
extern void led_color(unsigned int num, unsigned int rgb);
extern void led_color(unsigned int num, unsigned char red, unsigned char green, unsigned char blue);


#define CHANNELS  127

static unsigned char ch[CHANNELS];
static int state=0;
static int chindex=0;
static int refresh_needed=1;
static elapsedMicros transmittimer;

void leds_update()
{
  int num, avail;

  switch (state) {
  case 0: // idle
//    if (refresh_needed == 0) return;
    Serial1.begin(83333, SERIAL_8N1);
    Serial1.write(0);
    transmittimer = 0;
    refresh_needed = 0;
    state = 1;
    break;
  case 1: // waiting for break
    if (transmittimer < 150) return;
    Serial1.flush(); // should already be done
    Serial1.begin(250000, SERIAL_8N2);
    transmittimer = 0;
    Serial1.write(0);
    chindex = 0;
    state = 2;
    break;
  case 2: // transmit data
    avail = Serial1.availableForWrite();
    if (avail == 0) return;
    num = CHANNELS - chindex;
    if (num > avail) num = avail;
    Serial1.write(ch + chindex, num);
    chindex += num;
    if (chindex >= CHANNELS) state = 3;
    break;
  case 3: // waiting to transmit to complete
    if (transmittimer < CHANNELS * 44 + 100) return;
    Serial1.flush(); // should already be done
    state = 0;
    break;
  default:
    Serial1.begin(250000, SERIAL_8N2);
    state = 0;
  }
}

void led_channel(unsigned int num, unsigned int val)
{
  if (num == 0) return;  // LEDs start numbering at 1
  if (val > 255) val = 255;
  ch[num - 1] = val;
  refresh_needed = 1;
}

void led_color(unsigned int num, unsigned int rgb)
{
  led_color(num, rgb >> 16, rgb >> 8, rgb);
}

void led_color(unsigned int num, unsigned char red, unsigned char green, unsigned char blue)
{
  if (num == 0) return;  // LEDs start numbering at 1
  num = (num - 1) * 3;
  if (num + 2 >= CHANNELS) return;
  if (ch[num] != red) {
    ch[num] = red;
    refresh_needed = 1;
  }
  if (ch[num+1] != green) {
    ch[num+1] = green;
    refresh_needed = 1;
  }
  if (ch[num+2] != blue) {
    ch[num+2] = blue;
    refresh_needed = 1;
  }
}



