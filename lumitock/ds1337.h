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

#ifndef LUMI_DS1337_H
#define LUMI_DS1337_H

#include <stdint.h>

#define SECONDS_MASK   0x7F
#define MINUTES_MASK   0x7F
#define HOURS_MASK_12H 0x1F
#define HOURS_MASK_24H 0x3F

#define HOURS_FLAG_PM  (1 << 5)
#define HOURS_FLAG_12H (1 << 6)

typedef struct ds1337_time
{
  uint8_t h;
  uint8_t m;
  uint8_t s;
};

struct ds1337_time getTime();

void setTime(struct ds1337_time time);

struct ds1337_time plusMinute(struct ds1337_time time);

struct ds1337_time plusHour(struct ds1337_time time);

void printTime(struct ds1337_time time);

#endif
