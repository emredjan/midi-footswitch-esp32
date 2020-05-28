#include <Arduino.h>

#include <JC_Button.h>
#include <ShiftRegister74HC595.h>
#include <MIDI.h>

#include <displays.h>
#include <presets.h>
#include <commands.h>
#include <midiclock.h>

MIDI_CREATE_DEFAULT_INSTANCE();


/*** Button Things ***/

const byte BUTTON_PINS[] = {2, 4, 16, 17, 18, 19, 23};

const unsigned int LONG_PRESS = 600;

Button button1(BUTTON_PINS[0]);
Button button2(BUTTON_PINS[1]);
Button button3(BUTTON_PINS[2]);
Button button4(BUTTON_PINS[3]);
Button button5(BUTTON_PINS[4]);
Button button6(BUTTON_PINS[5]);
Button button7(BUTTON_PINS[6]);


/*** Shift Register Things ***/

const byte SR_NUM_REGISTERS = 3;
const byte SR_SDI_PIN = 13;
const byte SR_SCLK_PIN = 33;
const byte SR_LOAD_PIN = 32;

ShiftRegister74HC595<SR_NUM_REGISTERS> sr(SR_SDI_PIN, SR_SCLK_PIN, SR_LOAD_PIN);


/*** MIDI Utilities ***/

const byte MIN_BANK = 1;
const byte MAX_BANK = 5;


//TODO: Move MIDI Clock handling to separate file
/*** MIDI Clock Handling ***/


unsigned long bpm = 120;
unsigned long prevBpm = 120;
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


void tapTempo(ShiftRegister74HC595<3> sr)
{

    if (tapCounter == 0 && !midiClockState)
    {
        MIDI.sendRealTime(midi::MidiType::Stop);
        midiClockState = !midiClockState;
        MIDI.sendRealTime(midi::MidiType::Start);
    }

    for (byte i = 4; i > 0; i--)
        currentTimer[i] = currentTimer[i - 1];

    currentTimer[0] = micros() - lastTap;
    lastTap = micros();

    tapCounter++;

    if (tapCounter >= 3)
    {

        byte numAverage = tapCounter - 1;
        unsigned long totalTimer = 0UL;

        for (byte i = 0; i < numAverage; i++)
            totalTimer += currentTimer[i];

        usPerTap = totalTimer / numAverage;
        bpm = (unsigned long)60e6 / usPerTap;
        tapCounter--;
    }

    byte displayPrint[3];

    if (bpm != prevBpm)
    {
        for (int i = 0; i < 3; i++)
            displayPrint[i] = getNumberToPrint(bpm)[i];
        sr.setAll(displayPrint);
    }

    setDisplay(F("BPM"), F("CLK ON"));
}

void handleMidiClock()
{
    if (bpm != prevBpm)
    {
        usPerTick = (unsigned long)(1e6 / (bpm * 24.0 / 60.0));
        prevBpm = bpm;
    }

    if (micros() - prevTime >= usPerTick)
    {
        prevTime += usPerTick;
        if (bpm >= MIN_BPM && bpm <= MAX_BPM)
        {
            MIDI.sendRealTime(midi::MidiType::Clock);
        }
    }
}

//TODO: Tempo leds
void handleTempoLed(ShiftRegister74HC595<3> sr)
{
    if (micros() - prevBlink >= usPerTick * 12)
    {
        ledState = !ledState;
        prevBlink += usPerTick * 12;
        // set 7 segment dots for tempo display
        // sr.set(7, !ledState);
        // sr.set(15, !ledState);
        // sr.set(23, !ledState);
    }
}



void setup()
{

    for (byte i = 0; i < NUM_LEDS; i++)
        pinMode(LED_PINS[i], OUTPUT);

    MIDI.begin(MIDI_CHANNEL_OMNI);

    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();
    button5.begin();
    button6.begin();
    button7.begin();

    // Start by displaying a dash
    byte displayPrint[3] = {B10111111, B10111111, B10111111};
    sr.setAll(displayPrint);

}

void loop()
{

    /*** Handle MIDI Clock ***/

    if (tapCounter > 0 && micros() - lastTap > currentTimer[0] * 2)
        tapCounter = 0;

    if (midiClockState)
    {
        handleMidiClock();
        handleTempoLed(sr);
    }

    /*** Handle Buttons ***/

    enum states_t
    {
        WAIT,
        SHORT_1,
        SHORT_2,
        SHORT_3,
        SHORT_4,
        SHORT_5,
        SHORT_6,
        SHORT_7,
        TO_LONG_1,
        LONG_1,
        TO_LONG_2,
        LONG_2,
        TO_LONG_3,
        LONG_3,
        TO_LONG_4,
        LONG_4,
        TO_LONG_7,
        LONG_7
    };

    static states_t STATE = WAIT;

    static byte bankNum = 1;
    static byte newBankNum = 0;

    static bool commandMode = false;

    button1.read();
    button2.read();
    button3.read();
    button4.read();
    button5.read();
    button6.read();
    button7.read();

    switch (STATE)
    {
    case WAIT:
        if (button1.wasReleased())
            STATE = SHORT_1;
        else if (button2.wasReleased())
            STATE = SHORT_2;
        else if (button3.wasReleased())
            STATE = SHORT_3;
        else if (button4.wasReleased())
            STATE = SHORT_4;
        else if (button5.wasReleased())
            STATE = SHORT_5;
        else if (button6.wasReleased())
            STATE = SHORT_6;
        else if (button7.wasReleased())
            STATE = SHORT_7;
        else if (button1.pressedFor(LONG_PRESS))
            STATE = TO_LONG_1;
        else if (button2.pressedFor(LONG_PRESS))
            STATE = TO_LONG_2;
        else if (button3.pressedFor(LONG_PRESS))
            STATE = TO_LONG_3;
        else if (button4.pressedFor(LONG_PRESS))
            STATE = TO_LONG_4;
        else if (button7.pressedFor(LONG_PRESS))
            STATE = TO_LONG_7;
        break;

    case SHORT_1:
        if (commandMode)
            callCommand(1, sr);
        else
            callPreset(bankNum, 1, sr, MIDI);
        STATE = WAIT;
        break;

    case SHORT_2:
        if (commandMode)
            callCommand(2, sr);
        else
            callPreset(bankNum, 2, sr, MIDI);
        STATE = WAIT;
        break;

    case SHORT_3:
        if (commandMode)
            callCommand(3, sr);
        else
            callPreset(bankNum, 3, sr, MIDI);
        STATE = WAIT;
        break;

    case SHORT_4:
        if (commandMode)
            callCommand(4, sr);
        else
            callPreset(bankNum, 4, sr, MIDI);
        STATE = WAIT;
        break;

    case SHORT_5:
        callCommand(5, sr);
        STATE = WAIT;
        break;

    case SHORT_6:
        callCommand(6, sr);
        STATE = WAIT;
        break;

    case SHORT_7:
        // callCommand(7, sr);
        tapTempo(sr);
        STATE = WAIT;
        break;

    case TO_LONG_1:
        if (!commandMode)
        {
            setDisplay(F("CMD"));
            callCommand(0, sr);
            for (byte i = 0; i < 4; i++)
                command_sent[i] = false;
        }
        else
        {
            setDisplay(F("READY"));
            callPreset(bankNum, 0, sr, MIDI);
        }

        if (button1.wasReleased())
            STATE = LONG_1;
        break;

    case LONG_1:
        commandMode = !commandMode;
        STATE = WAIT;
        break;

    case TO_LONG_2:
        callCommand(0, sr);
        if (button2.wasReleased())
            STATE = LONG_2;
        break;

    case LONG_2:
        callCommand(8, sr);
        STATE = WAIT;
        break;

    case TO_LONG_3:
        if (commandMode)
        {
            if (button3.wasReleased())
                callCommand(3, sr);
            STATE = WAIT;
        }
        else
        {
            newBankNum = _min(bankNum + 1, MAX_BANK);
            callPreset(newBankNum, 0, sr, MIDI);
            if (button3.wasReleased())
                STATE = LONG_3;
        }
        break;

    case LONG_3:
        bankNum = newBankNum;
        STATE = WAIT;
        break;

    case TO_LONG_4:
        if (commandMode)
        {
            if (button4.wasReleased())
                callCommand(4, sr);
            STATE = WAIT;
        }
        else
        {
            newBankNum = _max(bankNum - 1, MIN_BANK);
            callPreset(newBankNum, 0, sr, MIDI);
            if (button4.wasReleased())
                STATE = LONG_4;
        }
        break;

    case LONG_4:
        bankNum = newBankNum;
        STATE = WAIT;
        break;

    case TO_LONG_7:
        if (button7.wasReleased())
            STATE = LONG_7;
        break;

    case LONG_7:
        byte displayPrint[3];
        for (int i = 0; i < 3; i++)
            displayPrint[i] = getNumberToPrint(bpm)[i];
        sr.setAll(displayPrint);

        if (midiClockState)
        {
            //MIDI.sendRealTime(MIDI_RT_STOP);
            setDisplay(F("BPM"), F("CLK OFF"));
        }
        else
        {
            //MIDI.sendRealTime(MIDI_RT_START);
            setDisplay(F("BPM"), F("CLK ON"));
        }
        midiClockState = !midiClockState;
        STATE = WAIT;
        break;
    }
}
