#include <Arduino.h>

#include <expression.h>

#include <globals.h>


void handleExpression(byte controller, byte channel)
{
    static unsigned long previousValue = 0b10000000;

    unsigned long analogValue = analogRead(EXPR_PIN);
    unsigned long midiValue = analogValue / 32;

    signalFilter.AddValue(midiValue);

    unsigned long filteredValue = signalFilter.GetLowPass();

    byte ccValue = map(filteredValue, 0, 118, 0, 127);

    if (ccValue != previousValue)
    {
        MIDI.sendControlChange(controller, ccValue, channel);

        // Debug prints
        // Serial.print("Analog: ");
        // Serial.print(analogValue);
        // Serial.print(" / MIDI: ");
        // Serial.print(midiValue);
        // Serial.print(" / Filtered: ");
        // Serial.print(filteredValue);
        // Serial.print(" / Final: ");
        // Serial.println(ccValue);

        previousValue = ccValue;
    }

}
