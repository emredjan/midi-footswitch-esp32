#include <Arduino.h>
#include <displays.h>
#include <ShiftRegister74HC595.h>

const byte LED_PINS[] = {14, 27, 26, 25};
const byte NUM_LEDS = 4;

const int FLICKER_NONE = 300;
const int FLICKER_SLOW = 100;
const int FLICKER_FAST = 60;

const byte numberB[] = {
    B11000000, //0
    B11111001, //1
    B10100100, //2
    B10110000, //3
    B10011001, //4
    B10010010, //5
    B10000010, //6
    B11111000, //7
    B10000000, //8
    B10010000  //9
};

void setDisplay(const __FlashStringHelper *msg)
{

}

void setDisplay(const __FlashStringHelper *msg1, const __FlashStringHelper *msg2)
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

    if (bpm_ >= 20 && bpm_ <= 300)
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


}

void msgFlicker(long flickerTime, int flickerCount, byte *message, ShiftRegister74HC595<3> sr)
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
                sr.setAll(message);
            }
            else
            {
                sr.setAllHigh();
            }

            previousBlink = millis();

            timesFlickered++;
        }
    }
}
