#include <Tlc5940.h>
#include <Wire.h>

#include "buttons.h"
#include "ds1337.h"
#include "leds.h"
#include "util.h"

static ds1337_time before;

void doStartupPattern()
{
  uint8_t maxv = 230;
  uint8_t split = maxv / 2;

  for (uint8_t i = 0; i < maxv; i += 4)
  {
    Tlc.clear();

    for (uint8_t j = 0; j < 16; ++j)
    {
      uint8_t ii = i > split ? split - (i - split) : i;
      Tlc.set(j, ii * 16);
      Tlc.set(j + 16, ii * 16);
    }

    Tlc.update();
    delay(15);
  }
}

void setup()
{
  Tlc.init();
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Clock initialized");
  randomSeed(analogRead(0));

  setupButtons();
  
  doStartupPattern();
  before = getTime();
}

void loop()
{
    static boolean first = true;
    ds1337_time now = getTime();
    boolean leds[16] = { false };

    if (now.s != before.s || now.m != before.m || now.h != before.h)
    {
      printTime(now);
      setLeds(now);
      before = now;
    }
    
    checkButtons();
}

