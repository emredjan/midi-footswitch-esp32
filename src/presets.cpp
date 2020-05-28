#include <Arduino.h>
#include <ShiftRegister74HC595.h>
#include <MIDI.h>

#include <displays.h>
#include <presets.h>

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

void callPreset(byte bank, byte program, ShiftRegister74HC595<3> sr, midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI)
{

    msgFlicker(FLICKER_FAST, 5, getNumberToPrint(bank, program), sr);

    byte displayPrint[3];
    for (int i = 0; i < 3; i++)
        displayPrint[i] = getNumberToPrint(bank, program)[i];
    sr.setAll(displayPrint);

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
            preset_1_1(MIDI);
            break;
        }
    }

    for (byte i = 0; i < 8; i++)
        command_sent[i] = false;
}

void preset_1_1(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI)
{
    MIDI.sendProgramChange(0, CH_IRIDIUM); // Iridium Clean

    MIDI.sendProgramChange(101, CH_SWITCHER); // 0 Acoustic Sim
    MIDI.sendProgramChange(102, CH_SWITCHER); // 0 Compressor
    MIDI.sendProgramChange(103, CH_SWITCHER); // 0 Booster
    MIDI.sendProgramChange(106, CH_SWITCHER); // 0 BE-OD

    MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
    MIDI.sendProgramChange(5 - 1, CH_EQ);      // EQ Bypass

    setDisplay(F("CLEAN"), F("GENERIC"));
}
