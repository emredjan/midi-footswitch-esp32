#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <Arduino.h>

#include <globals.h>
#include <commands.h>
#include <presets.h>
#include <displays.h>

void setupButtons();

void handleBuiltinButtons(AceButton *button, byte eventType, byte buttonState);
void handleExternalButtons(AceButton *button, byte eventType, byte buttonState);

#endif
