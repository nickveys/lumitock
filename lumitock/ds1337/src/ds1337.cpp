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

#include <Arduino.h>
#include <Wire.h>

#include <ds1337.h>
#include <ds1337_util.h>

#define DS1337_ADDR 0x68

struct ds1337_time getTime()
{
  Wire.beginTransmission(DS1337_ADDR);
  Wire.write(0);
  Wire.endTransmission();

  struct ds1337_time time;

  Wire.requestFrom(DS1337_ADDR, 3);
  time.s = Wire.read();
  time.m = Wire.read();
  time.h = Wire.read();
  
  return time;
}

void setTime(struct ds1337_time time)
{
  /* tidy up the values */
  time.s &= SECONDS_MASK;
  time.m &= MINUTES_MASK;
  time.h &= HOURS_MASK_12H;
  time.h |= HOURS_FLAG_12H;

  /* send the update */
  Wire.beginTransmission(DS1337_ADDR);
  Wire.write(0);
  Wire.write(time.s);
  Wire.write(time.m);
  Wire.write(time.h);
  Wire.endTransmission();
}

struct ds1337_time plusMinute(struct ds1337_time time)
{
  uint8_t m = bcd2dec(time.m & MINUTES_MASK);
  time.m = MINUTES_MASK & dec2bcd(++m % 60);

  return time;
}

struct ds1337_time plusHour(struct ds1337_time time)
{
  uint8_t h = bcd2dec(time.h & HOURS_MASK_12H);

  ++h;
  if (h > 12)
  {
    h = 1;
  }

  time.h = HOURS_MASK_12H & dec2bcd(h);
  return time;
}

void printTime(struct ds1337_time time)
{
  uint8_t s = bcd2dec(time.s & SECONDS_MASK);
  uint8_t m = bcd2dec(time.m & MINUTES_MASK);
  uint8_t h = bcd2dec(time.h & HOURS_MASK_12H);

  Serial.print("Current time: ");
  Serial.print(h, DEC);
  Serial.print(":");
  if (m < 10) Serial.print("0");
  Serial.print(m, DEC);
  Serial.print(":");
  if (s < 10) Serial.print("0");
  Serial.println(s, DEC);
}

