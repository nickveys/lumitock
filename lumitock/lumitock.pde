#include <Tlc5940.h>
#include <Wire.h>

#include "ds1337.h"
#include "leds.h"
#include "util.h"

#define BUTTON_1 7
#define BUTTON_2 6

#define BUTTON_DOWN 0
#define BUTTON_UP !BUTTON_DOWN

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

  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  
  doStartupPattern();
  before = getTime();
}

void doButton1()
{
  Serial.println("Button 1 pressed!");
  setTime(plusMinute(getTime()));
}

void doButton2()
{
  Serial.println("Button 2 pressed!");
  setTime(plusHour(getTime()));
}

void checkButtons()
{
  static uint8_t was1 = 0;
  static uint8_t was2 = 0;

  if (!digitalRead(BUTTON_1) && !was1)
  {
    delay(10);
    if (!digitalRead(BUTTON_1))
    {
      was1 = 1;
      doButton1();
    }
  }
  
  if (digitalRead(BUTTON_1) && was1)
  {
    was1 = 0;
  }

  if (!digitalRead(BUTTON_2) && !was2)
  {
    delay(10);
    if (!digitalRead(BUTTON_2))
    {
      was2 = 1;
      doButton2();
    }
  }
  
  if (digitalRead(BUTTON_2) && was2)
  {
    was2 = 0;
  }
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

