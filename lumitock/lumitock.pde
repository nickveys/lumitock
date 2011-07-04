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

