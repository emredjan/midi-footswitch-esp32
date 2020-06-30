#ifndef DISPLAYS_H_
#define DISPLAYS_H_

#include <Arduino.h>
#include <ShiftRegister74HC595.h>

void setOled(const char *msg);
void setOled(const char *msg1, const char *msg2);

byte *getNumberToPrint(byte bank, byte program, bool longPress);
byte *getNumberToPrint(unsigned int bpm_);

void msgFlicker(long flickerTime, int flickerCount, byte *message);

#endif
