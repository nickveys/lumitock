#include <WProgram.h>

#include "buttons.h"
#include "ds1337.h"

#define BUTTON_1 7
#define BUTTON_2 6

#define BUTTON_DOWN 0
#define BUTTON_UP !BUTTON_DOWN

void setupButtons()
{
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
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
