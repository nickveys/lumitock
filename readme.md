# Lumitock
Lumitock is an LED-grid clock.  Each digit is represented by a grid of LEDs, the quantity lit indicates the time for that segment.

## Images, other information
* [Blog post about prototype](http://veys.com/2011/09/26/an-open-source-led-grid-clock)

## Materials
* QTLP670C LEDs
* DS1337C RTC
* TLC5940 LED controllers
* Arduino Mini Pro

## Project Details
* Built on Arduino Uno
* Built using Arduino IDE
* Uses [TLC5940 library](http://code.google.com/p/tlc5940arduino)
 * Modify the tlc_config.h and set NUM_TLCS to 2
* Schematics and layouts in [EAGLE](http://www.cadsoft.de)
 * Edit EAGLE directories and add root of this project

