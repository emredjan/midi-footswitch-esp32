#include <Arduino.h>

#include <commands.h>
#include <displays.h>
#include <midiclock.h>
#include <globals.h>

void callCommand(byte program)
{

    for (byte i = 0; i < NUM_LEDS; i++)
    {
        if ((i == program - 1) && program != 0)
            digitalWrite(LED_PINS[i], HIGH);
        else
            digitalWrite(LED_PINS[i], LOW);
    }

    if (program != 7) // tap
        msgFlicker(FLICKER_FAST, 5, getNumberToPrint(0, program));

    switch (program)
    {
    case 1:
        command_1();
        break;
    case 2:
        command_2();
        break;
    case 3:
        command_3();
        break;
    case 4:
        command_4();
        break;
    case 5:
        command_5();
        break;
    case 6:
        command_6();
        break;
    case 7:
        command_7();
        break;
    case 8:
        command_8();
        break;
    }

    if (program != 7) // tap
    {
        byte displayPrint[3];
        for (int i = 0; i < 3; i++)
            displayPrint[i] = getNumberToPrint(0, program)[i];
        sevenSeg.setAll(displayPrint);
    }
}

void command_1()
{
    if (!command_sent[1 - 1])
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
    command_sent[1 - 1] = !command_sent[1 - 1];
}

void command_2()
{
    if (!command_sent[2 - 1])
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
    command_sent[2 - 1] = !command_sent[2 - 1];
}

void command_3()
{
    if (!command_sent[3 - 1])
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
    command_sent[3 - 1] = !command_sent[3 - 1];
}

void command_4()
{
    if (!command_sent[4 - 1])
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
    command_sent[4 - 1] = !command_sent[4 - 1];
}

void command_5()
{
    if (!command_sent[5 - 1])
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
    command_sent[5 - 1] = !command_sent[5 - 1];
}

void command_6()
{
    if (!command_sent[6 - 1])
    {
        MIDI.sendProgramChange(2, CH_MODFACTOR); // ModFactor Flanger
        setOled("CHORS", "ON");
    }
    else
    {
        MIDI.sendProgramChange(125, CH_MODFACTOR); // ModFactor Bypass
        setOled("CHORS", "OFF");
    }
    command_sent[6 - 1] = !command_sent[6 - 1];
}


void command_7()
{
    tapTempo();

    byte displayPrint[3];

    if (bpm != prevBpm)
    {
        for (int i = 0; i < 3; i++)
            displayPrint[i] = getNumberToPrint(bpm)[i];
        sevenSeg.setAll(displayPrint);
    }

    setOled("BPM", "CLK ON");
}

void command_8()
{
    if (!command_sent[8 - 1])
    {
        MIDI.sendProgramChange(113, CH_SWITCHER); // Booster
        setOled("BOOST", "ON");
    }
    else
    {
        MIDI.sendProgramChange(103, CH_SWITCHER); // Booster
        setOled("BOOST", "OFF");
    }
    command_sent[8 - 1] = !command_sent[8 - 1];
}
