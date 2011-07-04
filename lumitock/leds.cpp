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

#include <stdint.h>

#include <Tlc5940.h>

#include "leds.h"

uint16_t BRIGHTNESS_LEVELS[4] =
{
  1000,
  2000,
  3000,
  4000,
};

uint8_t brightness = 3;

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

boolean hasCountSet(boolean *indices, uint8_t size, uint8_t count)
{
  for (uint8_t i = 0; i < size; ++i)
  {
    count = indices[i] ? count - 1 : count;
  }

  return 0 == count;
}

void setRandomIndex(boolean *indices, uint8_t size)
{
  indices[random(size)] = true;
}

void setSegment(uint8_t ch0, uint8_t count, uint8_t value)
{
  boolean indices[16] = { false };

  while (!hasCountSet(indices, count, value))
  {
    setRandomIndex(indices, count);
  }

  for (uint8_t i = 0; i < count; ++i)
  {
    Tlc.set(i + ch0, indices[i] ? BRIGHTNESS_LEVELS[brightness] : 0);
  }
}

void setLeds(struct ds1337_time time)
{
  /* set all values to zero */
  Tlc.clear();
  
  setSegment(25, 2, (time.h & 0x30) >> 4);
  setSegment(16, 9, time.h & 0x0F);
  setSegment(9, 6, (time.m & 0x70) >> 4);
  setSegment(0, 9, time.m & 0x0F);

  /* apply LED values */
  Tlc.update();
}

