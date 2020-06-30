#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <Arduino.h>

#include <ShiftRegister74HC595.h>
#include <MIDI.h>
#include <SingleEMAFilterLib.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <AceButton.h>
using namespace ace_button;


/*** MIDI Object ***/

extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI;


/*** Expression ***/

extern SingleEMAFilter<int> signalFilter1;
// extern SingleEMAFilter<int> signalFilter2;

extern bool expressionEnabled[];
extern byte expressionCC[];
extern byte expressionChannel[];
extern const byte EXPR_PINS[];

extern const byte expressionCCDefault[];
extern const byte expressionChannelDefault[];

/*** Button Things ***/

extern const byte BUTTON_PINS[];
extern const byte NUM_BUTTONS;
extern const byte NUM_BUILTIN_BUTTONS;

extern const unsigned int DEBOUNCE_DELAY;
extern const unsigned int CLICK_DELAY;
extern const unsigned int LONG_PRESS_DELAY;
extern const unsigned int REPEAT_PRESS_DELAY;
extern const unsigned int REPEAT_PRESS_INTERVAL;

extern AceButton buttons[];
extern ButtonConfig buttonConfigs[];

extern byte bankNum;
extern byte newBankNum;
extern bool commandMode;


/** display related **/

extern const byte LED_PINS[];
extern const size_t NUM_LEDS;

extern const int FLICKER_NONE;
extern const int FLICKER_SLOW;
extern const int FLICKER_FAST;

extern const byte numberB[];

// extern U8G2_SH1106_128X64_NONAME_F_HW_I2C display;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C display;


/*** MIDI Object ***/


/** preset & command related **/

extern bool commandSent[9];
extern bool commandSentLong[9];

extern const byte CH_MODFACTOR;
extern const byte CH_IRIDIUM;
extern const byte CH_EQ;
extern const byte CH_SWITCHER ;

extern const byte SW_RELAYS_USED[];
extern const byte SW_RELAYS_UNUSED[];
extern const size_t NUM_RELAYS_USED;
extern const size_t NUM_RELAYS_UNUSED;

extern const char * BANK_NAMES[];


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

extern volatile unsigned long bpm;
extern volatile unsigned long prevBpm;
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
