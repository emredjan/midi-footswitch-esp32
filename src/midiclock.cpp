#include <Arduino.h>
#include <midiclock.h>

#include <MIDI.h>

#include <globals.h>
#include <displays.h>


void tapTempo()
{

    if (tapCounter == 0 && !midiClockState)
    {
        MIDI.sendRealTime(midi::MidiType::Stop);
        midiClockState = !midiClockState;
        MIDI.sendRealTime(midi::MidiType::Start);
    }

    for (byte i = 4; i > 0; i--)
        currentTimer[i] = currentTimer[i - 1];

    currentTimer[0] = micros() - lastTap;
    lastTap = micros();

    tapCounter++;

    if (tapCounter >= 3)
    {

        byte numAverage = tapCounter - 1;
        unsigned long totalTimer = 0UL;

        for (byte i = 0; i < numAverage; i++)
            totalTimer += currentTimer[i];

        usPerTap = totalTimer / numAverage;
        bpm = (unsigned long)60e6 / usPerTap;
        bpm = _min(_max(bpm, MIN_BPM), MAX_BPM); // limit BPM Range
        tapCounter--;
    }

}

void handleMidiClock()
{
    if (bpm != prevBpm)
    {
        usPerTick = (unsigned long)(1e6 / (bpm * 24.0 / 60.0));
        prevBpm = bpm;
    }

    if (micros() - prevTime >= usPerTick)
    {
        prevTime += usPerTick;
        MIDI.sendRealTime(midi::MidiType::Clock);
    }
}

//TODO: Tempo leds
void handleTempoLed()
{
    if (micros() - prevBlink >= usPerTick * 12)
    {
        ledState = !ledState;
        prevBlink += usPerTick * 12;
        // set 7 segment dots for tempo display
        sevenSeg.set(7, !ledState);
        sevenSeg.set(15, !ledState);
        sevenSeg.set(23, !ledState);
    }
}
