#include <Arduino.h>
#include <displays.h>
#include <MIDI.h>

extern bool command_sent[8];

extern const byte CH_MODFACTOR;
extern const byte CH_IRIDIUM;
extern const byte CH_EQ;
extern const byte CH_SWITCHER ;

void callPreset(byte bank, byte program, ShiftRegister74HC595<3> sr, midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI);

void preset_1_1(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI);
