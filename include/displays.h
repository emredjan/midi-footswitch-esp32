#ifndef DISPLAYS_H_
#define DISPLAYS_H_

#include <Arduino.h>
#include <ShiftRegister74HC595.h>

void setOled(const __FlashStringHelper *msg);
void setOled(const __FlashStringHelper *msg1, const __FlashStringHelper *msg2);

byte *getNumberToPrint(byte bank, byte program);
byte *getNumberToPrint(unsigned int bpm_);

void msgFlicker(long flickerTime, int flickerCount, byte *message);


#endif
