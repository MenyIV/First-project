# First-project

ARuino adaptible termostat with heat and cold function.
ou can control temperature with two circuit Cold and Heat. It is simple. Just set temperature and regulator make all.

Part list:

LCD 16X2
ARduino NANO
double relay board
DS18 thermosensor
DHT11 thermo+hummidity
3xLED (R;G;B)
KY-040 torgue button

Sensors:
sensor DH11 works for short information.

Two sensors DS18 are in work. This check temperature and correcting heater/cooling. 
First sensor check temperature and give 1 and 2 temperature. If you connect second temperature sensor (I use waterproof sensor for check temperature into fluid) this sensor give the first temperature used for correction.


DISPLAY 1 base:
-------------------------------------------------------------------------------------------------------------
Teplota1 IN           Vnějšítplota/vlhkost
Teplota2 ININ         nastavená teplota
-------------------------------------------------------------------------------------------------------------

DISPLAY 2 statistics:
-------------------------------------------------------------------------------------------------------------
MIN
MAX
-------------------------------------------------------------------------------------------------------------

DISPLAY 3 temp set:
-------------------------------------------------------------------------------------------------------------
Nastav teplotu (temperature setting):
Temperature1 IN         setting temp
-------------------------------------------------------------------------------------------------------------
