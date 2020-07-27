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
        MIDI.sendProgramChange(7, CH_H9); // H9 Subtle Flange
        setOled("FLNGR", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_H9;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
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
        MIDI.sendProgramChange(3, CH_H9); // H9 Phaser
        setOled("PHASR", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_H9;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
        setOled("PHASR", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSent[2] = !commandSent[2];
}

void command_2L() // Boost
{
    if (!commandSentLong[2])
    {
        // TODO: Booster
        setOled("BOOST", "ON");
    }
    else
    {
        // TODO: Booster Off
        setOled("BOOST", "OFF");
    }
    commandSentLong[2] = !commandSentLong[2];
}

void command_3() // Rotary
{
    if (!commandSent[3])
    {
        MIDI.sendProgramChange(10, CH_H9); // H9 Rotary
        setOled("ROTRY", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_H9;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
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
        MIDI.sendProgramChange(11, CH_H9); // H9 Tremolo
        setOled("TREM", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_H9;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
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
        MIDI.sendProgramChange(2, CH_H9); // H9 Chorus 70s
        setOled("CHR70", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
        setOled("CHR70", "OFF");
    }
    commandSent[5] = !commandSent[5];
}

void command_5L() // Chorus 2
{
    if (!commandSentLong[5])
    {
        MIDI.sendProgramChange(1, CH_H9); // H9 Chorus 80s
        setOled("CHR80", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
        setOled("CHR80", "OFF");
    }
    commandSentLong[5] = !commandSentLong[5];
}

void command_6() // Uvibe
{
    if (!commandSent[6])
    {
        MIDI.sendProgramChange(4, CH_H9); // H9 Uni-Vibe
        setOled("UVIBE", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
        setOled("UVIBE", "OFF");
    }
    commandSent[6] = !commandSent[6];
}

void command_6L() // Something weird? Formant?
{
    if (!commandSentLong[6])
    {
        MIDI.sendProgramChange(9, CH_H9); // H9 Auto Filter
        setOled("WEIRD", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
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
        MIDI.sendProgramChange(5, CH_H9); // H9 Vintage Wah
        setOled("VWAH", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_H9;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
        setOled("VWAH", "OFF");

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
        MIDI.sendProgramChange(6, CH_H9); // H9 Talk Wah
        setOled("TWAH", "ON");

        expressionEnabled[0] = true;
        expressionChannel[0] = CH_H9;
        expressionCC[0] = 11;
    }
    else
    {
        MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
        setOled("TWAH", "OFF");

        expressionEnabled[0] = true;
        expressionChannel[0] = expressionChannelDefault[0];
        expressionCC[0] = expressionCCDefault[0];
    }
    commandSentLong[8] = !commandSentLong[8];
}
