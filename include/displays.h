#include <Arduino.h>
#include <ShiftRegister74HC595.h>

extern const byte LED_PINS[];
extern const byte NUM_LEDS;

extern const int FLICKER_NONE;
extern const int FLICKER_SLOW;
extern const int FLICKER_FAST;

extern const byte numberB[];

void setDisplay(const __FlashStringHelper *msg);
void setDisplay(const __FlashStringHelper *msg1, const __FlashStringHelper *msg2);

byte *getNumberToPrint(byte bank, byte program);
byte *getNumberToPrint(unsigned int bpm_);

void msgFlicker(long flickerTime, int flickerCount, byte *message, ShiftRegister74HC595<3> sr);
