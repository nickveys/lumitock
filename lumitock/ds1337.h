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

#define SECONDS_MASK_UPPER    0x70
#define SECONDS_MASK_LOWER    0x0F
#define MINUTES_MASK_UPPER    0x70
#define MINUTES_MASK_LOWER    0x0F
#define HOURS_12H_MASK_UPPER  0x10
#define HOURS_12H_MASK_LOWER  0x0F
#define HOURS_24H_MASK_UPPER  0x30
#define HOURS_24H_MASK_LOWER  0x0F

#define SECONDS_MASK   (SECONDS_MASK_UPPER | SECONDS_MASK_LOWER)
#define MINUTES_MASK   (MINUTES_MASK_UPPER | MINUTES_MASK_LOWER)
#define HOURS_MASK_12H (HOURS_12H_MASK_UPPER | HOURS_12H_MASK_LOWER)
#define HOURS_MASK_24H (HOURS_24H_MASK_UPPER | HOURS_24H_MASK_LOWER)

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
