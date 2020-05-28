#include <Arduino.h>
#include <ShiftRegister74HC595.h>

#include <commands.h>
#include <displays.h>
#include <midiclock.h>


void callCommand(byte program, ShiftRegister74HC595<3> sr)
{

    for (byte i = 0; i < NUM_LEDS; i++)
    {
        if ((i == program - 1) && program != 0)
            digitalWrite(LED_PINS[i], HIGH);
        else
            digitalWrite(LED_PINS[i], LOW);
    }

    if (program != 7) // tap
        msgFlicker(FLICKER_FAST, 5, getNumberToPrint(0, program), sr);

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
        sr.setAll(displayPrint);
    }
}

void command_1()
{
}
void command_2()
{
}
void command_3()
{
}
void command_4()
{
}
void command_5()
{
}
void command_6()
{
}
void command_7()
{
}
void command_8()
{
}
