#include <Wire.h>

#include "ds1337.h"

#define DS1337_ADDR 0x68

struct ds1337_time getTime()
{
  Wire.beginTransmission(DS1337_ADDR);
  Wire.send(0);
  Wire.endTransmission();

  struct ds1337_time time;

  Wire.requestFrom(DS1337_ADDR, 3);
  time.s = Wire.receive();
  time.m = Wire.receive();
  time.h = Wire.receive();
  
  return time;
}

void setTime(struct ds1337_time time)
{
  Wire.beginTransmission(DS1337_ADDR);
  Wire.send(0);
  Wire.send(time.s);        // seconds
  Wire.send(time.m);        // minutes
  Wire.send(time.h | 0x40); // hours (12h clock)
  Wire.endTransmission();
}

