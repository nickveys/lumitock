#ifndef LUMI_DS1337_H
#define LUMI_DS1337_H

#include <stdint.h>

struct ds1337_time
{
  uint8_t h;
  uint8_t m;
  uint8_t s;
};

struct ds1337_time getTime();

void setTime(struct ds1337_time time);

#endif
