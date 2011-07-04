#ifndef LUMI_LEDS_H
#define LUMI_LEDS_H

#include <stdint.h>
#include <WProgram.h>

#include "ds1337.h"

boolean hasCountSet(boolean *indices, uint8_t size, uint8_t count);

void setRandomIndex(boolean *indices, uint8_t size);

void setSegment(uint8_t ch0, uint8_t count, uint8_t value);

void setLeds(struct ds1337_time time);

#endif
