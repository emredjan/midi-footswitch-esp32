# MIDI Footswitch with ESP32

## Features

- Uses ESP32 as the brains
- Built with PlatformIO, Arduino framework
- 3 digit 7-segment LED display for bank-program & BPM display
- 0.96" OLED screen for bank-program name & additional functions
- 2 duplicated MIDI outputs
- 4 fixed footswitches
- 4 fixed LEDs
- 2 external input jacks for 2-4 additional switches
- 2 expression pedal inputs
- Send any MIDI command via footswitches or expression
- Presets are saved up to 9 banks in 4 (36 in total)
- Send MIDI clock (Tap tempo on switch 7)

## Todo List

- [ ] Onboard preset editing
- [ ] Add a rotary encode for additional functions (may need pcb change)
- [ ] Move presets into NVS with Preferences Library
- [ ] Set repeat press events for bank change buttons
- [ ] OTA update
- [ ] Web based config
- [ ] OLED improvements, more information
- [ ] Dual core optimizations (Currently MIDI Clock handling runs on separate core, but hacky)
- [ ] Move LEDS / Switches to a shift register (not a priority, ESP32 has enough GPIO)

## 3rd Party Libraries

- **[Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library)**: General MIDI functionality
- **[AceButton](https://github.com/bxparks/AceButton)**: Switch handling
- **[ShiftRegister 74HC595 Arduino Library](https://github.com/Simsso/ShiftRegister74HC595)**: For driving the 7-segment LED display
- **[Arduino Single EMA Filter](https://github.com/luisllamasbinaburo/Arduino-SingleEmaFilter)**: Filtering voltage noise from analog inputs (for expression pedals)
- **[U8G2](https://github.com/olikraus/u8g2)**: Driving the OLED display
