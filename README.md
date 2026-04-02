# DeskPiston
Real life minecraft piston

## Important GPIO Disclaimer
Based on your board, some GPIO pins may not work for input pull-up/pull-down or servo PWM output.
Please reference the exact name of your board (for example: "ESP32 DevKit V1", "Raspberry Pi Pico", "XIAO RP2040") and check that board's pinout/notes before choosing pins.

## Wiring Standards (Practical)
- Momentary pushbutton is normally 2 terminals in this project:
	- one side to input GPIO
	- one side to GND (with internal pull-up enabled)
- You do not need "2 grounds" for one button input. One shared board ground is the normal standard.
- Lever/slide switch parts often have 3 terminals (SPDT), but this code reads one GPIO state:
	- 2-terminal wiring is fine (GPIO + GND, using pull-up)
	- 3-terminal SPDT can also be used; many builders connect one throw to GND and leave the other throw unused for simple two-state input
- Servo power standard:
	- servo signal from GPIO
	- servo power from suitable supply (often external 5V)
	- servo ground must be common with MCU ground

Simple RP2040/MicroPython rack-and-pinion control:
- Press button once -> servo extends.
- Press button again -> servo retracts.

## Wiring
- Button:
	- One side to GP15
	- Other side to GND
- Servo signal wire to GP16
- Servo GND to RP2040 GND
- Servo V+ to external 5V (recommended)

If you use an external 5V supply for the servo, connect the external GND to RP2040 GND.

## Run
1. Flash MicroPython on your RP2040 board.
2. Copy `main.py` to the board.
3. Reset board.

At the very top of `main.py`, users can set:
- `USE_TOGGLE_BUTTON` or `USE_LEVEL_SWITCH` (true/false style behavior selection)
- `CONTROL_PIN`
- `SERVO_PIN`
- input polarity and switch behavior options

## Arduino C++ Version
Use `DeskPiston_Arduino.ino` for Arduino-compatible boards (including many RP2040 cores).

Behavior is the same:
- First press extends servo.
- Second press retracts servo.

Default pins in the sketch:
- Button: `GPIO 15` to `GND` (uses `INPUT_PULLUP`)
- Servo signal: `GPIO 16`

In Arduino IDE:
1. Open `DeskPiston_Arduino.ino`.
2. Select board and port.
3. Upload.

At the very top of `DeskPiston_Arduino.ino`, users can set:
- `USE_TOGGLE_BUTTON` or `USE_LEVEL_SWITCH` (true/false behavior selection)
- `CONTROL_PIN`
- `SERVO_PIN`
- input polarity and switch behavior options

## Tuning
Edit these values in `main.py`:
- `CONTROL_PIN`
- `SERVO_PIN`
- `EXTENDED_ANGLE`
- `RETRACTED_ANGLE`
- `DEBOUNCE_MS`
