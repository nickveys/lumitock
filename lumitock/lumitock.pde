#include <Tlc5940.h>
#include <Wire.h>

#define DS1337_ADDR B01101000

#define BUTTON_1 7
#define BUTTON_2 6

#define BUTTON_DOWN 0
#define BUTTON_UP !BUTTON_DOWN

uint16_t BRIGHTNESS_LEVELS[4] =
{
  1000,
  2000,
  3000,
  4000,
};

uint8_t brightness = 0;

struct ds1337_time
{
  uint8_t h;
  uint8_t m;
  uint8_t s;
};

static ds1337_time before;

struct ds1337_time getTime();

void doStartupPattern()
{
  uint8_t maxv = 230;
  uint8_t split = maxv / 2;

  for (uint8_t i = 0; i < maxv; i += 4)
  {
    Tlc.clear();

    for (uint8_t j = 0; j < 16; ++j)
    {
      uint8_t ii = i > split ? split - (i - split) : i;
      Tlc.set(j, ii * 16);
      Tlc.set(j + 16, ii * 16);
    }

    Tlc.update();
    delay(15);
  }
}

void setup()
{
  Tlc.init();
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Clock initialized");
  randomSeed(analogRead(0));

  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);

  brightness = 3;
  
  doStartupPattern();
  before = getTime();
}

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

void setTime(ds1337_time time)
{
  Wire.beginTransmission(DS1337_ADDR);
  Wire.send(0);
  Wire.send(time.s);        // seconds
  Wire.send(time.m);        // minutes
  Wire.send(time.h | 0x40); // hours (12h clock)
  Wire.endTransmission();
}

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

uint8_t hasCountSet(uint8_t *indices, uint8_t size, uint8_t count)
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

void setLeds(ds1337_time time)
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

uint8_t dec2bcd(uint8_t dec)
{
  return ((dec / 10) << 4) + (dec % 10);
}

uint8_t bcd2dec(uint8_t bcd)
{
  return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

ds1337_time plusMinute(ds1337_time time)
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

ds1337_time plusHour(ds1337_time time)
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

void printTime(ds1337_time time)
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

void doButton1()
{
  Serial.println("Button 1 pressed!");
  setTime(plusMinute(getTime()));
}

void doButton2()
{
  Serial.println("Button 2 pressed!");
  setTime(plusHour(getTime()));
}

void checkButtons()
{
  static uint8_t was1 = 0;
  static uint8_t was2 = 0;

  if (!digitalRead(BUTTON_1) && !was1)
  {
    delay(10);
    if (!digitalRead(BUTTON_1))
    {
      was1 = 1;
      doButton1();
    }
  }
  
  if (digitalRead(BUTTON_1) && was1)
  {
    was1 = 0;
  }

  if (!digitalRead(BUTTON_2) && !was2)
  {
    delay(10);
    if (!digitalRead(BUTTON_2))
    {
      was2 = 1;
      doButton2();
    }
  }
  
  if (digitalRead(BUTTON_2) && was2)
  {
    was2 = 0;
  }
}

void loop()
{
    static boolean first = true;
    ds1337_time now = getTime();
    boolean leds[16] = { false };

    if (now.s != before.s || now.m != before.m || now.h != before.h)
    {
      printTime(now);
      setLeds(now);
      before = now;
    }
    
    checkButtons();
}

