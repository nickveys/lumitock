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

#ifndef LUMI_LEDS_H
#define LUMI_LEDS_H

#include <stdint.h>
#include <WProgram.h>

#include "ds1337.h"

boolean hasCountSet(uint16_t indices, uint8_t size, uint8_t count);

void setRandomIndex(uint16_t *indices, uint8_t size);

uint16_t setRandomSegment(uint8_t ch0, uint8_t count, uint8_t value);

void setLeds(struct ds1337_time time);

#endif
