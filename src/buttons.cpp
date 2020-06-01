#include <Arduino.h>

#include <buttons.h>

void setupButtons()
{

    // Setup Commong Features
    for (size_t i = 0; i < NUM_BUTTONS; i++)
    {
        // Setup and init button
        pinMode(BUTTON_PINS[i], INPUT);
        buttons[i].init(&buttonConfigs[i], BUTTON_PINS[i], HIGH, i);

        buttonConfigs[i].setDebounceDelay(DEBOUNCE_DELAY);
        buttonConfigs[i].setFeature(ButtonConfig::kFeatureClick);
        buttonConfigs[i].setFeature(ButtonConfig::kFeatureSuppressAfterClick);

        // Faster click delay for tap tempo (button 7)
        if (i == 6)
            buttonConfigs[i].setClickDelay(CLICK_DELAY - 60);
        else
            buttonConfigs[i].setClickDelay(CLICK_DELAY);

        // No Long Click for buttons 5 & 6
        if (i != 4 && i != 5)
        {
            buttonConfigs[i].setFeature(ButtonConfig::kFeatureLongPress);
            buttonConfigs[i].setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
            buttonConfigs[i].setLongPressDelay(LONG_PRESS_DELAY);
        }

        // Add repeat press for bank change buttons (3 & 4)
        if (i == 2 || i == 3)
        {
            buttonConfigs[i].setFeature(ButtonConfig::kFeatureRepeatPress);
            buttonConfigs[i].setFeature(ButtonConfig::kFeatureSuppressAfterRepeatPress);
            buttonConfigs[i].setRepeatPressDelay(REPEAT_PRESS_DELAY);
            buttonConfigs[i].setRepeatPressInterval(REPEAT_PRESS_INTERVAL);
        }
    }

    // Set button handling functions for builtin buttons
    for (size_t i = 0; i < NUM_BUILTIN_BUTTONS; i++)
        buttonConfigs[i].setEventHandler(handleBuiltinButtons);

    // Set button handling functions for external buttons
    for (size_t i = NUM_BUILTIN_BUTTONS; i < NUM_BUTTONS; i++)
        buttonConfigs[i].setEventHandler(handleExternalButtons);

}

void handleBuiltinButtons(AceButton *button, byte eventType, byte buttonState)
{

    byte buttonNumber = button->getId() + 1;

    switch (eventType)
    {
    case AceButton::kEventReleased:
        if (commandMode)
            callCommand(buttonNumber);
        else
            callPreset(bankNum, buttonNumber);
        break;
    case AceButton::kEventLongPressed:
        switch (buttonNumber)
        {
        case 1:
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
            commandMode = !commandMode;
            break;
        case 2:
            callCommand(0);
            callCommand(8);
            break;
        case 3:
            if (commandMode)
                callCommand(3);
            else
            {
                newBankNum = _min(bankNum + 1, MAX_BANK);
                callPreset(newBankNum, 0);
                bankNum = newBankNum;
            }
            break;
        case 4:
            if (commandMode)
                callCommand(4);
            else
            {
                newBankNum = _max(bankNum - 1, MIN_BANK);
                callPreset(newBankNum, 0);
                bankNum = newBankNum;
            }
            break;
        }
        break;
    }
}

void handleExternalButtons(AceButton *button, byte eventType, byte buttonState)
{

    byte buttonNumber = button->getId() + 1;

    switch (eventType)
    {
    case AceButton::kEventReleased:
        callCommand(buttonNumber);
        break;
    case AceButton::kEventLongPressed:
        switch (buttonNumber)
        {
        case 7:
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
            break;
        }
        break;
    }
}
