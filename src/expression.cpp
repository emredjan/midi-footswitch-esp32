#include <Arduino.h>

#include <expression.h>

#include <globals.h>


void setupExpression()
{
    analogReadResolution(12);
    analogSetPinAttenuation(EXPR_PINS[0], ADC_11db);
    // analogSetPinAttenuation(EXPR_PINS[1], ADC_11db);

    expressionEnabled[0] = true;
    // expressionEnabled[1] = false;
}

void handleExpression1(byte controller, byte channel)
{
    static unsigned long previousValue1 = 0b10000000;

    unsigned long analogValue = analogRead(EXPR_PINS[0]);
    unsigned long midiValue = analogValue / 32;

    signalFilter1.AddValue(midiValue);

    unsigned long filteredValue = signalFilter1.GetLowPass();

    byte ccValue = map(filteredValue, 0, 118, 0, 127);

    if (ccValue != previousValue1)
    {
        MIDI.sendControlChange(controller, ccValue, channel);
        previousValue1 = ccValue;

        // Debug prints
        // Serial.print("Analog: ");
        // Serial.print(analogValue);
        // Serial.print(" / MIDI: ");
        // Serial.print(midiValue);
        // Serial.print(" / Filtered: ");
        // Serial.print(filteredValue);
        // Serial.print(" / Final: ");
        // Serial.println(ccValue);

    }

}


// void handleExpression2(byte controller, byte channel)
// {
//     static unsigned long previousValue2 = 0b10000000;

//     unsigned long analogValue = analogRead(EXPR_PINS[1]);
//     unsigned long midiValue = analogValue / 32;

//     signalFilter2.AddValue(midiValue);

//     unsigned long filteredValue = signalFilter2.GetLowPass();

//     byte ccValue = map(filteredValue, 0, 118, 0, 127);

//     if (ccValue != previousValue2)
//     {
//         MIDI.sendControlChange(controller, ccValue, channel);
//         previousValue2 = ccValue;

//         // Debug prints
//         // Serial.print("Analog: ");
//         // Serial.print(analogValue);
//         // Serial.print(" / MIDI: ");
//         // Serial.print(midiValue);
//         // Serial.print(" / Filtered: ");
//         // Serial.print(filteredValue);
//         // Serial.print(" / Final: ");
//         // Serial.println(ccValue);

//     }

// }
