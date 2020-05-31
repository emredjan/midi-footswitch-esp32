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
const byte expressionChannelDefault[] = {CH_IRIDIUM, CH_MODFACTOR};

/*** Button Things ***/

const byte BUTTON_PINS[] = {36, 39, 34, 35, 25, 26, 27};

const unsigned int LONG_PRESS = 600;

Button button1(BUTTON_PINS[0], 25, false, true);
Button button2(BUTTON_PINS[1], 25, false, true);
Button button3(BUTTON_PINS[2], 25, false, true);
Button button4(BUTTON_PINS[3], 25, false, true);
Button button5(BUTTON_PINS[4], 25, false, true);
Button button6(BUTTON_PINS[5], 25, false, true);
Button button7(BUTTON_PINS[6], 25, false, true);

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

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE);

/** preset & command related **/

bool command_sent[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/*
    Channel  1: Eventide ModFactor
    Channel  2: Strymon Iridium
    *Channel  3: Strymon Compadre
    Channel  4: Source Audio Programmable EQ
    *Channel  5: Source Audio Collider
    Channel 16: MIDI Switcher
*/

const byte CH_MODFACTOR = 1;
const byte CH_IRIDIUM = 2;
const byte CH_EQ = 4;
const byte CH_SWITCHER = 16;

const byte SW_RELAYS_USED[] = {1, 2, 3, 6};
const byte SW_RELAYS_UNUSED[] = {4, 5, 7, 8};
const size_t NUM_RELAYS_USED = sizeof(SW_RELAYS_USED) / sizeof(SW_RELAYS_USED[0]);
const size_t NUM_RELAYS_UNUSED = sizeof(SW_RELAYS_UNUSED) / sizeof(SW_RELAYS_UNUSED[0]);


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
