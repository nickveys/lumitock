#include <stdint.h>

#include "util.h"

uint8_t dec2bcd(uint8_t dec)
{
  return ((dec / 10) << 4) + (dec % 10);
}

uint8_t bcd2dec(uint8_t bcd)
{
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

