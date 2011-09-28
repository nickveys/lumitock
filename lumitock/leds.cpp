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

#include <stdint.h>

#include "leds.h"

uint16_t BRIGHTNESS_LEVELS[4] =
{
  1000,
  2000,
  3000,
  4000,
};

uint8_t brightness = 0;

void cycleBrightness()
{
  if (brightness == 3)
  {
    brightness = 0;
  }
  else
  {
    ++brightness;
  }
}

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

boolean hasCountSet(uint16_t indices, uint8_t size, uint8_t count)
{
  for (uint8_t i = 0; i < size; ++i)
  {
    count = (indices & (1 << i)) ? count - 1 : count;
  }

  return 0 == count;
}

void setRandomIndex(uint16_t *indices, uint8_t size)
{
  (*indices) |= (1 << random(size));
}

uint16_t setSegment(uint8_t ch0, uint8_t count, uint16_t indices, boolean fadeIn)
{
  uint16_t maxval = BRIGHTNESS_LEVELS[brightness];
  uint32_t t0 = millis() + 50;
  uint32_t t1 = t0 + 500;

  for (uint8_t i = 0; i < count; ++i)
  {
    if (fadeIn)
    {
      tlc_addFade(i + ch0, 0, (indices & (1 << i)) ? maxval : 0, t0, t1);
    }
    else
    {
      tlc_addFade(i + ch0, (indices & (1 << i)) ? maxval : 0, 0, t0, t1);
    }
  }

  return indices;
}

uint16_t setRandomSegment(uint8_t ch0, uint8_t count, uint8_t value)
{
  uint16_t indices = 0;

  while (!hasCountSet(indices, count, value))
  {
    setRandomIndex(&indices, count);
  }

  return setSegment(ch0, count, indices, true);
}

void setLeds(struct ds1337_time time)
{
  static boolean first = true;
  static uint16_t hu, hl, mu, ml;

  if (!first)
  {
    setSegment(16, 3, hu, false);
    setSegment(19, 9, hl, false);
    setSegment(0, 6, mu, false);
    setSegment(6, 9, ml, false);
    while (tlc_updateFades());
  }
  else
  {
    Tlc.clear();
    first = false;
  }

  hu = setRandomSegment(16, 3, (time.h & HOURS_12H_MASK_UPPER) >> 4);
  hl = setRandomSegment(19, 9, time.h & HOURS_12H_MASK_LOWER);
  mu = setRandomSegment(0, 6, (time.m & MINUTES_MASK_UPPER) >> 4);
  ml = setRandomSegment(6, 9, time.m & MINUTES_MASK_LOWER);
  while (tlc_updateFades());
}

