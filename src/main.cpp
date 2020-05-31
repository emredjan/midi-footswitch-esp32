#include <Arduino.h>

#include <globals.h>
#include <displays.h>
#include <presets.h>
#include <commands.h>
#include <midiclock.h>
#include <expression.h>

TaskHandle_t clockTask;

void clockTaskCode(void *pvParameters)
{
    for (;;)
    {
        /*** Handle MIDI Clock ***/

        if (tapCounter > 0 && micros() - lastTap > currentTimer[0] * 2)
            tapCounter = 0;

        if (midiClockState)
        {
            handleMidiClock();
            handleTempoLed();
            vTaskDelay(1);
        }
        else
        {
            vTaskDelay(10);
        }
    }
}

void setup()
{
    // Create the task on core 0 for MIDI Clock handling
    xTaskCreatePinnedToCore(clockTaskCode, "clockTask", 1000, NULL, 10, &clockTask, 0);

    setupExpression();

    for (byte i = 0; i < NUM_LEDS; i++)
        pinMode(LED_PINS[i], OUTPUT);

    display.begin();
    setOled("READY");

    Serial.begin(115200);
    MIDI.begin(MIDI_CHANNEL_OMNI);

    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();
    button5.begin();
    button6.begin();
    button7.begin();

    // Disable Unused Switcher Relays
    for (byte i = 0; i < NUM_RELAYS_UNUSED; i++)
        MIDI.sendProgramChange(100 + SW_RELAYS_UNUSED[i], CH_SWITCHER);

    // Start by displaying a dash
    byte displayPrint[3] = {B10111111, B10111111, B10111111};
    sevenSeg.setAll(displayPrint);
}

void loop()
{

    /** Handle Expression Pedals **/

    if (expressionEnabled[0])
        handleExpression1(expressionCC[0], expressionChannel[0]);

    // if (expressionEnabled[1])
    //     handleExpression2(expressionCC[1], expressionChannel[1]);

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
            callCommand(1);
        else
            callPreset(bankNum, 1);
        STATE = WAIT;
        break;

    case SHORT_2:
        if (commandMode)
            callCommand(2);
        else
            callPreset(bankNum, 2);
        STATE = WAIT;
        break;

    case SHORT_3:
        if (commandMode)
            callCommand(3);
        else
            callPreset(bankNum, 3);
        STATE = WAIT;
        break;

    case SHORT_4:
        if (commandMode)
            callCommand(4);
        else
            callPreset(bankNum, 4);
        STATE = WAIT;
        break;

    case SHORT_5:
        callCommand(5);
        STATE = WAIT;
        break;

    case SHORT_6:
        callCommand(6);
        STATE = WAIT;
        break;

    case SHORT_7:
        callCommand(7);
        STATE = WAIT;
        break;

    case TO_LONG_1:
        if (!commandMode)
        {
            setOled("CMD");
            callCommand(0);
            for (byte i = 0; i < 4; i++)
                command_sent[i] = false;
        }
        else
        {
            setOled("READY");
            callPreset(bankNum, 0);
        }

        if (button1.wasReleased())
            STATE = LONG_1;
        break;

    case LONG_1:
        commandMode = !commandMode;
        STATE = WAIT;
        break;

    case TO_LONG_2:
        callCommand(0);
        if (button2.wasReleased())
            STATE = LONG_2;
        break;

    case LONG_2:
        callCommand(8);
        STATE = WAIT;
        break;

    case TO_LONG_3:
        if (commandMode)
        {
            if (button3.wasReleased())
                callCommand(3);
            STATE = WAIT;
        }
        else
        {
            newBankNum = _min(bankNum + 1, MAX_BANK);
            callPreset(newBankNum, 0);
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
                callCommand(4);
            STATE = WAIT;
        }
        else
        {
            newBankNum = _max(bankNum - 1, MIN_BANK);
            callPreset(newBankNum, 0);
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
        sevenSeg.setAll(displayPrint);

        if (midiClockState)
        {
            MIDI.sendRealTime(midi::MidiType::Stop);
            midiClockState = !midiClockState;
            setOled("BPM", "CLK OFF");
        }
        else
        {
            MIDI.sendRealTime(midi::MidiType::Start);
            midiClockState = !midiClockState;
            setOled("BPM", "CLK ON");
        }
        STATE = WAIT;
        break;
    }
}
