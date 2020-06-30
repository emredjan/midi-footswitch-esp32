#include <Arduino.h>

#include <commands.h>
#include <displays.h>
#include <midiclock.h>
#include <globals.h>

void callCommand(byte program, bool longPress)
{

    for (byte i = 0; i < NUM_LEDS; i++)
    {
        if ((i == program - 1) && program != 0)
            digitalWrite(LED_PINS[i], HIGH);
        else
            digitalWrite(LED_PINS[i], LOW);
    }

    if (program != 7) // tap
        msgFlicker(FLICKER_FAST, 5, getNumberToPrint(0, program, longPress));

    switch (program)
    {
    case 1:
        command_1();
        break;
    case 2:
        if (longPress)
            command_2L();
        else
            command_2();
        break;
    case 3:
        command_3();
        break;
    case 4:
        command_4();
        break;
    case 5:
        if (longPress)
            command_5L();
        else
            command_5();
        break;
    case 6:
        if (longPress)
            command_6L();
        else
            command_6();
        break;
    case 7:
        if (longPress)
            command_7L();
        else
            command_7();
        break;
    case 8:
        if (longPress)
            command_8L();
        else
            command_8();
        break;
    }

    if (program != 7) // tap
    {
        byte displayPrint[3];
        for (int i = 0; i < 3; i++)
            displayPrint[i] = getNumberToPrint(0, program, longPress)[i];
        sevenSeg.setAll(displayPrint);
    }
}

void command_1() // Flanger etc.
{
    if (!commandSent[1])
    {
        MIDI.sendProgramChange(7, CH_MODFACTOR); // ModFactor Chorus
        setOled("FLNGR", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_MODFACTOR;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("FLNGR", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSent[1] = !commandSent[1];
}

void command_2() // Phaser
{
    if (!commandSent[2])
    {
        MIDI.sendProgramChange(3, CH_MODFACTOR); // ModFactor Phaser
        setOled("PHASR", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_MODFACTOR;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("PHASR", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSent[2] = !commandSent[2];
}

void command_2L() // Uvibe? Boost?
{
    if (!commandSentLong[2])
    {
        MIDI.sendProgramChange(113, CH_SWITCHER); // Booster
        setOled("BOOST", "ON");
    }
    else
    {
        MIDI.sendProgramChange(103, CH_SWITCHER); // Booster
        setOled("BOOST", "OFF");
    }
    commandSentLong[2] = !commandSentLong[2];
}

void command_3() // Rotary
{
    if (!commandSent[3])
    {
        MIDI.sendProgramChange(11, CH_MODFACTOR); // ModFactor Rotary
        setOled("ROTRY", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_MODFACTOR;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("ROTRY", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSent[3] = !commandSent[3];
}

void command_4() // Tremolo
{
    if (!commandSent[4])
    {
        MIDI.sendProgramChange(13, CH_MODFACTOR); // ModFactor Tremolo
        setOled("TREM", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_MODFACTOR;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("TREM", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSent[4] = !commandSent[4];
}

void command_5() // Chorus
{
    if (!commandSent[5])
    {
        MIDI.sendProgramChange(2, CH_MODFACTOR); // ModFactor Flanger
        setOled("CHORS", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("CHORS", "OFF");
    }
    commandSent[5] = !commandSent[5];
}

void command_5L() // Chorus 2
{
    if (!commandSentLong[5])
    {
        MIDI.sendProgramChange(2, CH_MODFACTOR); // ModFactor Flanger
        setOled("CHRS2", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("CHRS2", "OFF");
    }
    commandSentLong[5] = !commandSentLong[5];
}

void command_6() // Uvibe? Boost?
{
    if (!commandSent[6])
    {
        MIDI.sendProgramChange(2, CH_MODFACTOR); // ModFactor Flanger
        setOled("UVIBE", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("UVIBE", "OFF");
    }
    commandSent[6] = !commandSent[6];
}

void command_6L() // Something weird? Formant?
{
    if (!commandSentLong[6])
    {
        MIDI.sendProgramChange(2, CH_MODFACTOR); // ModFactor Flanger
        setOled("WEIRD", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("WEIRD", "OFF");
    }
    commandSentLong[6] = !commandSentLong[6];
}

void command_7() // Tap Tempo
{
    tapTempo();

    byte displayPrint[3];

    if (bpm != prevBpm)
    {
        for (int i = 0; i < 3; i++)
            displayPrint[i] = getNumberToPrint(bpm)[i];
        sevenSeg.setAll(displayPrint);
    }

    setOled("BPM", "CLOCK ON");
}

void command_7L() // MIDI Clock start stop
{
    byte displayPrint[3];
    for (int i = 0; i < 3; i++)
        displayPrint[i] = getNumberToPrint(bpm)[i];
    sevenSeg.setAll(displayPrint);

    if (midiClockState)
    {
        MIDI.sendRealTime(midi::MidiType::Stop);
        midiClockState = !midiClockState;
        setOled("BPM", "CLCK OFF");
    }
    else
    {
        MIDI.sendRealTime(midi::MidiType::Start);
        midiClockState = !midiClockState;
        setOled("BPM", "CLOCK ON");
    }
}

void command_8() // Wah
{
    if (!commandSent[8])
    {
        MIDI.sendProgramChange(5, CH_MODFACTOR); // ModFactor Wah
        setOled("WAH", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_MODFACTOR;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("WAH", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSent[8] = !commandSent[8];
}

void command_8L() // Wah 2
{
    if (!commandSentLong[8])
    {
        MIDI.sendProgramChange(5, CH_MODFACTOR); // ModFactor Wah
        setOled("WAH2", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_MODFACTOR;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("WAH2", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSentLong[8] = !commandSentLong[8];
}
