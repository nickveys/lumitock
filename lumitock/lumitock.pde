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

/* Give plenty of room for the fade buffer */
#define TLC_FADE_BUFFER_LENGTH 64

#include <Tlc5940.h>
#include <tlc_fades.h>
#include <Wire.h>

#include "buttons.h"
#include "ds1337.h"
#include "leds.h"
#include "util.h"

static ds1337_time before;

void doStartupPattern()
{
  uint8_t maxv = 2000;
  uint16_t dur = 350;

  uint8_t cols[][3] =
  {
    /* H[0] */ {16, 17, 18},
    /* H[1] */ {19, 20, 21}, {22, 23, 24}, {25, 26, 27},
    /* M[0] */ {0, 1, 2}, {3, 4, 5},
    /* M[1] */ {6, 7, 8}, {9, 10, 11}, {12, 13, 14}
  };

  for (uint8_t i = 0; i < 9; ++i)
  {
    uint32_t startms = millis() + 50 * i;
    uint32_t endms = startms + dur;

    tlc_addFade(cols[i][0], 0, maxv, startms, endms);
    tlc_addFade(cols[i][0], maxv, 0, endms, endms + dur);
    tlc_addFade(cols[8 - i][1], 0, maxv, startms, endms);
    tlc_addFade(cols[8 - i][1], maxv, 0, endms, endms + dur);
    tlc_addFade(cols[i][2], 0, maxv, startms, endms);
    tlc_addFade(cols[i][2], maxv, 0, endms, endms + dur);
  }
  while (tlc_updateFades());

  for (uint8_t i = 0; i < 9; ++i)
  {
    uint32_t startms = millis() + 50 * i;
    uint32_t endms = startms + dur;

    tlc_addFade(cols[8 - i][0], 0, maxv, startms, endms);
    tlc_addFade(cols[8 - i][0], maxv, 0, endms, endms + dur);
    tlc_addFade(cols[i][1], 0, maxv, startms, endms);
    tlc_addFade(cols[i][1], maxv, 0, endms, endms + dur);
    tlc_addFade(cols[8 - i][2], 0, maxv, startms, endms);
    tlc_addFade(cols[8 - i][2], maxv, 0, endms, endms + dur);
  }
  while (tlc_updateFades());
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

