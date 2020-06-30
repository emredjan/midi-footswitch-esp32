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
        buttonConfigs[i].setClickDelay(CLICK_DELAY);

        buttonConfigs[i].setFeature(ButtonConfig::kFeatureLongPress);
        buttonConfigs[i].setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
        buttonConfigs[i].setLongPressDelay(LONG_PRESS_DELAY);

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

    if (eventType == AceButton::kEventReleased)
    {
        if (commandMode)
            callCommand(buttonNumber, 0);
        else
            callPreset(bankNum, buttonNumber);
    }
    else
    {
        switch (buttonNumber)
        {
        case 1:
            if (eventType == AceButton::kEventLongPressed)
            {
                if (!commandMode)
                {
                    setOled("CMD");
                    callCommand(0, 0);
                    for (byte i = 1; i < NUM_BUILTIN_BUTTONS + 1; i++)
                    {
                        commandSent[i] = false;
                        commandSentLong[i] = false;
                    }
                }
                else
                {
                    setOled("READY");
                    callPreset(bankNum, 0);
                }
                commandMode = !commandMode;
            }
            break;
        case 2:
            if (eventType == AceButton::kEventLongPressed)
            {
                callCommand(0, 0);
                callCommand(2, 1);
            }
            break;
        case 3:
            if (commandMode && eventType == AceButton::kEventLongPressed)
                callCommand(3, 0);
            else if (!commandMode && eventType == AceButton::kEventRepeatPressed)
            {
                newBankNum = _min(bankNum + 1, MAX_BANK);
                callPreset(newBankNum, 0);
                bankNum = newBankNum;
            }
            break;
        case 4:
            if (commandMode && eventType == AceButton::kEventLongPressed)
                callCommand(4, 0);
            else if (!commandMode && eventType == AceButton::kEventRepeatPressed)
            {
                newBankNum = _max(bankNum - 1, MIN_BANK);
                callPreset(newBankNum, 0);
                bankNum = newBankNum;
            }
            break;
        }
    }
}

void handleExternalButtons(AceButton *button, byte eventType, byte buttonState)
{

    byte buttonNumber = button->getId() + 1;

    switch (eventType)
    {
    case AceButton::kEventReleased:
        callCommand(buttonNumber, 0);
        break;
    case AceButton::kEventLongPressed:
        callCommand(buttonNumber, 1);
        break;
    }
}
