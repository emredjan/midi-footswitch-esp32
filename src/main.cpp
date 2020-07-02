#include <Arduino.h>

#include <globals.h>
#include <displays.h>
#include <presets.h>
#include <commands.h>
#include <midiclock.h>
#include <expression.h>
#include <buttons.h>

TaskHandle_t clockTask;

void clockTaskCode(void *pvParameters);


void setup()
{
    // Create the task on core 0 for MIDI Clock handling
    xTaskCreatePinnedToCore(clockTaskCode, "clockTask", 1000, NULL, 10, &clockTask, 0);

    setupExpression();
    setupButtons();

    for (byte i = 0; i < NUM_LEDS; i++)
        pinMode(LED_PINS[i], OUTPUT);

    display.begin();
    setOled("READY");

    Serial.begin(115200);
    MIDI.begin(MIDI_CHANNEL_OMNI);

    // Start by displaying a dash
    byte displayPrint[3] = {B10111111, B10111111, B10111111};
    sevenSeg.setAll(displayPrint);
}

void loop()
{
    /** Handle Buttons **/

    for (byte i = 0; i < NUM_BUTTONS; i++)
        buttons[i].check();

    /** Handle Expression Pedals **/

    if (expressionEnabled[0])
        handleExpression1(expressionCC[0], expressionChannel[0]);

    // if (expressionEnabled[1])
    //     handleExpression2(expressionCC[1], expressionChannel[1]);

}

void clockTaskCode(void *pvParameters)
{
    for (;;)
    {
        /*** Handle MIDI Clock ***/

        if (tapCounter > 0 && micros() - lastTap > currentTimer[0] * 2)
            tapCounter = 0;

        if (midiClockState)
        {
            handleMidiClock();
            handleTempoLed();
            vTaskDelay(1);
        }
        else
        {
            vTaskDelay(10);
        }
    }
}
