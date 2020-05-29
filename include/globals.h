#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <Arduino.h>

#include <JC_Button.h>
#include <ShiftRegister74HC595.h>
#include <MIDI.h>
#include <SingleEMAFilterLib.h>


/*** MIDI Object ***/

extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI;


/*** Expression ***/

extern SingleEMAFilter<int> signalFilter;

extern bool expressionEnabled;
extern byte expressionCC;
extern byte expressionChannel;
extern const byte EXPR_PIN;


/*** Button Things ***/

extern const byte BUTTON_PINS[] ;

extern const unsigned int LONG_PRESS;

extern Button button1;
extern Button button2;
extern Button button3;
extern Button button4;
extern Button button5;
extern Button button6;
extern Button button7;


/** display related **/

extern const byte LED_PINS[];
extern const byte NUM_LEDS;

extern const int FLICKER_NONE;
extern const int FLICKER_SLOW;
extern const int FLICKER_FAST;

extern const byte numberB[];



/*** MIDI Object ***/


/** preset & command related **/

extern bool command_sent[8];

extern const byte CH_MODFACTOR;
extern const byte CH_IRIDIUM;
extern const byte CH_EQ;
extern const byte CH_SWITCHER ;


/*** Shift Register Things ***/

extern const byte SR_NUM_REGISTERS;
extern const byte SR_SDI_PIN;
extern const byte SR_SCLK_PIN;
extern const byte SR_LOAD_PIN;

extern ShiftRegister74HC595<3> sevenSeg;


/*** MIDI Utilities ***/

extern const byte MIN_BANK;
extern const byte MAX_BANK;


/*** MIDI Clock Handling ***/

extern unsigned long bpm;
extern unsigned long prevBpm;
extern unsigned long usPerTick;
extern unsigned long prevTime;

extern const unsigned long MIN_BPM;
extern const unsigned long MAX_BPM;

extern unsigned long currentTimer[5];
extern unsigned long lastTap;

extern unsigned long usPerTap;
extern byte tapCounter;

extern bool midiClockState;

extern unsigned long prevBlink;
extern bool ledState;




#endif
