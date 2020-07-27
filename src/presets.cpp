#include <Arduino.h>

#include <displays.h>
#include <presets.h>
#include <globals.h>



void callPreset(byte bank, byte program)
{

    msgFlicker(FLICKER_FAST, 5, getNumberToPrint(bank, program, 0));

    byte displayPrint[3];
    for (int i = 0; i < 3; i++)
        displayPrint[i] = getNumberToPrint(bank, program, 0)[i];
    sevenSeg.setAll(displayPrint);

    if (program == 0 && bank >= 1 && bank <= 5)
    {
        String bankMsg = "BNK ";
        String bankNum_ = String(bank);
        bankMsg += bankNum_;
        setOled(bankMsg.c_str(), BANK_NAMES[bank - 1]);
    }


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
        case 2:
            preset_1_2();
            break;
        case 3:
            preset_1_3();
            break;
        case 4:
            preset_1_4();
            break;
        }
        break;
    case 2:
        switch (program)
        {
        case 1:
            preset_2_1();
            break;
        case 2:
            preset_2_2();
            break;
        case 3:
            preset_2_3();
            break;
        case 4:
            preset_2_4();
            break;
        }
        break;
    case 3:
        switch (program)
        {
        case 1:
            preset_3_1();
            break;
        case 2:
            preset_3_2();
            break;
        case 3:
            preset_3_3();
            break;
        case 4:
            preset_3_4();
            break;
        }
        break;
    case 4:
        switch (program)
        {
        case 1:
            preset_4_1();
            break;
        case 2:
            preset_4_2();
            break;
        case 3:
            preset_4_3();
            break;
        case 4:
            preset_4_4();
            break;
        }
        break;
    case 5:
        switch (program)
        {
        case 1:
            preset_5_1();
            break;
        case 2:
            preset_5_2();
            break;
        case 3:
            preset_5_3();
            break;
        case 4:
            preset_5_4();
            break;
        }
        break;
        // case 6:
        //     switch (program)
        //     {
        //     case 1:
        //         preset_6_1();
        //         break;
        //     case 2:
        //         preset_6_2();
        //         break;
        //     case 3:
        //         preset_6_3();
        //         break;
        //     case 4:
        //         preset_6_4();
        //         break;
        //     }
        //     break;
        // case 7:
        //     switch (program)
        //     {
        //     case 1:
        //         preset_7_1();
        //         break;
        //     case 2:
        //         preset_7_2();
        //         break;
        //     case 3:
        //         preset_7_3();
        //         break;
        //     case 4:
        //         preset_7_4();
        //         break;
        //     }
        //     break;
        // case 8:
        //     switch (program)
        //     {
        //     case 1:
        //         preset_8_1();
        //         break;
        //     case 2:
        //         preset_8_2();
        //         break;
        //     case 3:
        //         preset_8_3();
        //         break;
        //     case 4:
        //         preset_8_4();
        //         break;
        //     }
        //     break;
        // case 9:
        //     switch (program)
        //     {
        //     case 1:
        //         preset_9_1();
        //         break;
        //     case 2:
        //         preset_9_2();
        //         break;
        //     case 3:
        //         preset_9_3();
        //         break;
        //     case 4:
        //         preset_9_4();
        //         break;
        //     }
        //     break;
    }

    for (byte i = 1; i < NUM_BUTTONS + 1; i++)
    {
        commandSent[i] = false;
        commandSentLong[i] = false;
    }
}

void preset_1_1()
{
    MIDI.sendProgramChange(0, CH_IRIDIUM); // Iridium Clean
    MIDI.sendProgramChange(1, CH_SWITCHER); // Switcher 1 On 2 Off
    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("CLEAN", BANK_NAMES[0]);

}

void preset_1_2()
{
    MIDI.sendProgramChange(1, CH_IRIDIUM); // Iridium Crunch
    MIDI.sendProgramChange(2, CH_SWITCHER); // Switcher 1 Off 2 On
    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("CRNCH", BANK_NAMES[0]);

}

void preset_1_3()
{
    MIDI.sendProgramChange(2, CH_IRIDIUM); // Iridium Pedal Platform


    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("DRIVE", BANK_NAMES[0]);
}

void preset_1_4()
{
    MIDI.sendProgramChange(2, CH_IRIDIUM); // Iridium Pedal Platform


    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(0, CH_EQ);      // EQ Lead Boost

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("LEAD", BANK_NAMES[0]);
}

void preset_2_1()
{
    MIDI.sendProgramChange(0, CH_IRIDIUM); // Iridium Clean



    MIDI.sendProgramChange(2, CH_H9); // H9 Chorus
    MIDI.sendProgramChange(4, CH_EQ);    // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("CLEAN", BANK_NAMES[1]);
}

void preset_2_2()
{
    MIDI.sendProgramChange(0, CH_IRIDIUM); // Iridium Clean



    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("ACSTC", BANK_NAMES[1]);
}

void preset_2_3()
{
    MIDI.sendProgramChange(3, CH_IRIDIUM); // Iridium Drive


    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("DRIVE", BANK_NAMES[1]);
}

void preset_2_4()
{
    MIDI.sendProgramChange(3, CH_IRIDIUM); // Iridium Drive



    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(0, CH_EQ);      // EQ Lead Boost

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("LEAD", BANK_NAMES[1]);
}

void preset_3_1()
{
    MIDI.sendProgramChange(0, CH_IRIDIUM); // Iridium Clean



    MIDI.sendProgramChange(1, CH_H9); // H9 Chorus (80s)
    MIDI.sendProgramChange(4, CH_EQ);    // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("CLEAN", BANK_NAMES[2]);
}

void preset_3_2()
{
    MIDI.sendProgramChange(2, CH_IRIDIUM); // Iridium Pedal Platform


    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(1, CH_EQ);      // EQ TubeScreamer

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("DRV1", BANK_NAMES[2]);
}

void preset_3_3()
{
    MIDI.sendProgramChange(4, CH_IRIDIUM); // Iridium High Gain



    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(1, CH_EQ);      // EQ TubeScreamer

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("DRV2", BANK_NAMES[2]);
}

void preset_3_4()
{
    MIDI.sendProgramChange(4, CH_IRIDIUM); // Iridium High Gain



    MIDI.sendProgramChange(1, CH_H9); // H9 Chorus (80s)
    MIDI.sendProgramChange(1, CH_EQ);    // EQ TubeScreamer

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("LEAD", BANK_NAMES[2]);
}

void preset_4_1()
{
    MIDI.sendProgramChange(5, CH_IRIDIUM); // Iridium Jangly


    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("CLEAN", BANK_NAMES[3]);
}

void preset_4_2()
{
    MIDI.sendProgramChange(5, CH_IRIDIUM); // Iridium Jangly


    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("SQSHD", BANK_NAMES[3]);
}

void preset_4_3()
{
    MIDI.sendProgramChange(5, CH_IRIDIUM); // Iridium Jangly



    MIDI.sendProgramChange(9, CH_H9); // H9 Auto Filter
    MIDI.sendProgramChange(4, CH_EQ);    // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = CH_H9;
    expressionCC[0] = 11;

    setOled("F WAH", BANK_NAMES[3]);
}

void preset_4_4()
{
    MIDI.sendProgramChange(5, CH_IRIDIUM); // Iridium Jangly



    MIDI.sendProgramChange(4, CH_H9); // H9 Uni-Vibe
    MIDI.sendProgramChange(4, CH_EQ);    // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = CH_H9;
    expressionCC[0] = 11;

    setOled("PHASD", BANK_NAMES[3]);
}

void preset_5_1()
{
    MIDI.sendProgramChange(3, CH_SWITCHER);    // Switcher All On
    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = true;
    expressionChannel[0] = expressionChannelDefault[0];
    expressionCC[0] = expressionCCDefault[0];

    setOled("SWTCH", "ALL ON");
}

void preset_5_2()
{
    MIDI.sendProgramChange(0, CH_SWITCHER);    // Switcher All Off
    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = false;

    setOled("SWTCH", "ALL OFF");
}

void preset_5_3()
{
    setOled("EMPTY", "");
}

void preset_5_4()
{
    MIDI.sendProgramChange(0, CH_SWITCHER); // Switcher All Off
    delay(1000);

    MIDI.sendProgramChange(125, CH_H9); // H9 Bypass
    MIDI.sendProgramChange(4, CH_EQ);      // EQ Bypass

    expressionEnabled[0] = false;
    midiClockState = false;

    setOled("SWTCH", "RESET");
}

// void preset_6_1()
// {
// }

// void preset_6_2()
// {
// }

// void preset_6_3()
// {
// }

// void preset_6_4()
// {
// }

// void preset_7_1()
// {
// }

// void preset_7_2()
// {
// }

// void preset_7_3()
// {
// }

// void preset_7_4()
// {
// }

// void preset_8_1()
// {
// }

// void preset_8_2()
// {
// }

// void preset_8_3()
// {
// }

// void preset_8_4()
// {
// }

// void preset_9_1()
// {
// }

// void preset_9_2()
// {
// }

// void preset_9_3()
// {
// }

// void preset_9_4()
// {
// }
