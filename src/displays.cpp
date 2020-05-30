#include <Arduino.h>

#include <displays.h>

#include <globals.h>

void drawMidiClock()
{
    // MIDI Clock Icon
    display.setFont(u8g2_font_open_iconic_app_1x_t);
    display.drawStr(106, 10, "H");
    display.setFont(u8g2_font_open_iconic_play_1x_t);
    if (midiClockState)
        display.drawStr(116, 10, "E"); // Play
    else
        display.drawStr(116, 10, "D"); // Pause
}


void setOled(const char *msg)
{
    display.clearBuffer();

    display.setFont(u8g2_font_inb27_mf);
    display.drawStr(5, 48, msg);

    drawMidiClock();

    display.sendBuffer();
}

void setOled(const char *msg1, const char *msg2)
{
    display.clearBuffer();

    display.setFont(u8g2_font_inb24_mf);
    display.drawStr(5, 38, msg1);

    display.setFont(u8g2_font_t0_22_me);
    display.drawStr(5, 60, msg2);

    drawMidiClock();

    display.sendBuffer();
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
