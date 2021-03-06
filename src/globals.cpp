#include <Arduino.h>

#include <globals.h>


/*** MIDI Object ***/

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);


/*** Expression ***/

SingleEMAFilter<int> signalFilter1(0.1);
// SingleEMAFilter<int> signalFilter2(0.1);

bool expressionEnabled[] = {false, false};
byte expressionCC[] = {11, 12};
byte expressionChannel[] = {1, 2};
const byte EXPR_PINS[] = {32, 33};

const byte expressionCCDefault[] = {7, 11};
const byte expressionChannelDefault[] = {CH_IRIDIUM, CH_H9};

/*** Button Things ***/

const byte BUTTON_PINS[] = {36, 39, 34, 35, 25, 26, 27, 13};
const byte NUM_BUTTONS = sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]);
const byte NUM_BUILTIN_BUTTONS = 4;

const unsigned int DEBOUNCE_DELAY = 18;
const unsigned int CLICK_DELAY = 35;
const unsigned int LONG_PRESS_DELAY = 700;
const unsigned int REPEAT_PRESS_DELAY = 700;
const unsigned int REPEAT_PRESS_INTERVAL = 500;

AceButton buttons[NUM_BUTTONS];
ButtonConfig buttonConfigs[NUM_BUTTONS];

byte bankNum = 1;
byte newBankNum = 0;
bool commandMode = false;


/** display related **/

const byte LED_PINS[] = {4, 0, 2, 15};
const size_t NUM_LEDS = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

const int FLICKER_NONE = 300;
const int FLICKER_SLOW = 100;
const int FLICKER_FAST = 60;

const byte numberB[] = {
    B11000000, //0
    B11111001, //1
    B10100100, //2
    B10110000, //3
    B10011001, //4
    B10010010, //5
    B10000010, //6
    B11111000, //7
    B10000000, //8
    B10010000  //9
};

// U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

/** preset & command related **/

bool commandSent[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // to make it easy from 1
bool commandSentLong[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // to make it easy from 1

/*
    Channel  1: Strymon Iridium
    Channel  2: Strymon Compadre
    Channel  3: Strymon Sunset
    Channel  4: Source Audio Programmable EQ
    Channel  5: Eventide H9
    Channel  6: Source Audio Collider
    Channel 16: MIDI Switcher
*/

const byte CH_IRIDIUM = 1;
const byte CH_COMPADRE = 2;
const byte CH_SUNSET = 3;
const byte CH_EQ = 4;
const byte CH_H9 = 5;
const byte CH_COLLIDER = 6;
const byte CH_SWITCHER = 16;

const char * BANK_NAMES[] = {
    "GENERIC",
    "LOW GAIN",
    "HI GAIN",
    "FUNK/POP",
    "UTILITY"
};


/*** Shift Register Things ***/

const byte SR_NUM_REGISTERS = 3;
const byte SR_SDI_PIN = 19;
const byte SR_SCLK_PIN = 18;
const byte SR_LOAD_PIN = 5;

ShiftRegister74HC595<SR_NUM_REGISTERS> sevenSeg(SR_SDI_PIN, SR_SCLK_PIN, SR_LOAD_PIN);


/*** MIDI Utilities ***/

const byte MIN_BANK = 1;
const byte MAX_BANK = 5;


/*** MIDI Clock Handling ***/

volatile unsigned long bpm = 120;
volatile unsigned long prevBpm = 120;
unsigned long usPerTick = (unsigned long)(1e6 / (bpm * 24.0 / 60.0));
unsigned long prevTime = 0UL;

const unsigned long MIN_BPM = 30;
const unsigned long MAX_BPM = 300;

unsigned long currentTimer[5] = {1000000, 1000000, 1000000, 1000000, 1000000};
unsigned long lastTap = 0UL;

unsigned long usPerTap = 0UL;
byte tapCounter = 0;

bool midiClockState = false;

unsigned long prevBlink = 0UL;
bool ledState = 0;
