#include <Arduino.h>

#include <displays.h>
#include <presets.h>
#include <globals.h>



void callPreset(byte bank, byte program)
{

    msgFlicker(FLICKER_FAST, 5, getNumberToPrint(bank, program));

    byte displayPrint[3];
    for (int i = 0; i < 3; i++)
        displayPrint[i] = getNumberToPrint(bank, program)[i];
    sevenSeg.setAll(displayPrint);

    for (byte i = 0; i < NUM_LEDS; i++)
    {
        if ((i == program - 1) && program != 0)
            digitalWrite(LED_PINS[i], HIGH);
        else
            digitalWrite(LED_PINS[i], LOW);
    }

    switch (bank)
    {
    case 1:
        switch (program)
        {
        case 1:
            preset_1_1();
            break;
        }
    }

    for (byte i = 0; i < 8; i++)
        command_sent[i] = false;
}

void preset_1_1()
{
    MIDI.sendProgramChange(0, CH_IRIDIUM); // Iridium Clean

    MIDI.sendProgramChange(101, CH_SWITCHER); // 0 Acoustic Sim
    MIDI.sendProgramChange(102, CH_SWITCHER); // 0 Compressor
    MIDI.sendProgramChange(103, CH_SWITCHER); // 0 Booster
    MIDI.sendProgramChange(106, CH_SWITCHER); // 0 BE-OD

    MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
    MIDI.sendProgramChange(5 - 1, CH_EQ);      // EQ Bypass

    setOled(F("CLEAN"), F("GENERIC"));
}
