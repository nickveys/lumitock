#include <Wire.h>
#include <WProgram.h>

#include "ds1337.h"
#include "util.h"

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

struct ds1337_time plusMinute(struct ds1337_time time)
{
  uint8_t m = bcd2dec(time.m & 0x7F);

  ++m;
  if (m > 59)
  {
    m = 0;
  }
  
  time.m = 0x7F & dec2bcd(m);
  return time;
}

struct ds1337_time plusHour(struct ds1337_time time)
{
  uint8_t h = bcd2dec(time.h & 0x3F);
  
  ++h;
  if (h > 12)
  {
    h = 1;
  }
  
  time.h = 0x3F & dec2bcd(h);
  return time;
}

void printTime(struct ds1337_time time)
{
  uint8_t s = bcd2dec(time.s & 0x7F);
  uint8_t m = bcd2dec(time.m & 0x7F);
  uint8_t h = bcd2dec(time.h & 0x3F);

  Serial.print("Current time: ");
  Serial.print(h, DEC);
  Serial.print(":");
  Serial.print(m, DEC);
  Serial.print(":");
  Serial.println(s, DEC);
}
