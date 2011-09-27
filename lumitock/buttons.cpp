/**
 * This file is part of lumitock.
 *
 * Lumitock is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Lumitock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lumitock.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <WProgram.h>

#include "buttons.h"
#include "ds1337.h"

#define BUTTON_1 6
#define BUTTON_2 7

#define BUTTON_DOWN 0
#define BUTTON_UP !BUTTON_DOWN

void setupButtons()
{
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
}

void doButton1()
{
  setTime(plusMinute(getTime()));
}

void doButton2()
{
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
