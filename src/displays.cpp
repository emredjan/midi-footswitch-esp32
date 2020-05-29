#include <Arduino.h>

#include <displays.h>

#include <globals.h>


void setOled(const __FlashStringHelper *msg)
{

}

void setOled(const __FlashStringHelper *msg1, const __FlashStringHelper *msg2)
{

}



byte *getNumberToPrint(byte bank, byte program)
{
    static byte numberToPrint[3];

    if (bank == 0)
        numberToPrint[0] = B11000110;
    else
        numberToPrint[0] = numberB[bank];

    // Print a dash in the middle
    numberToPrint[1] = B10111111;

    if (program == 0)
        // Print a dash at the end if just a bank change command
        numberToPrint[2] = B10111111;
    else
        numberToPrint[2] = numberB[program];

    return numberToPrint;
}


byte *getNumberToPrint(unsigned int bpm_)
{

    static byte numberToPrint[3];

    if (bpm_ < 100)
        numberToPrint[0] = B11111111;
    else
        numberToPrint[0] = numberB[(bpm_ / 100) % 10];

    numberToPrint[1] = numberB[(bpm_ / 10) % 10];
    numberToPrint[2] = numberB[bpm_ % 10];

    return numberToPrint;

}

void msgFlicker(long flickerTime, int flickerCount, byte *message)
{
    unsigned long previousBlink = 0;
    bool flickState = false;
    int timesFlickered = 0;

    while (timesFlickered < flickerCount)
    {
        if (millis() - previousBlink >= flickerTime)
        {
            flickState = !flickState;
            if (flickState)
            {
                sevenSeg.setAll(message);
            }
            else
            {
                sevenSeg.setAllHigh();
            }

            previousBlink = millis();

            timesFlickered++;
        }
    }
}
