#include <Arduino.h>
#include <Servo.h>

// ===================== USER SETTINGS =====================
// Choose ONE control style by setting true/false below.
const bool USE_TOGGLE_BUTTON = true;   // Press once -> extend, press again -> retract
const bool USE_LEVEL_SWITCH = false;   // Lever/slide switch: one position extend, other retract

// Pins (change these to whatever GPIO you want to use)
const uint8_t CONTROL_PIN = 15;
const uint8_t SERVO_PIN = 9;

// Input polarity
const bool CONTROL_ACTIVE_LOW = true;  // true for switch/button wired from GPIO to GND (with pull-up)
// Engineering default: one control GPIO + one shared GND is standard for button input.
// 3-pin SPDT slider can still be used; this code reads a single logic state on CONTROL_PIN.

// Only used in lever/slide switch mode:
// true  -> active position means extended
// false -> active position means retracted
const bool SWITCH_ACTIVE_MEANS_EXTENDED = true;

// Servo positions
const int EXTENDED_ANGLE = 90;
const int RETRACTED_ANGLE = 0;

// Debounce timing
const unsigned long DEBOUNCE_MS = 25;
// ========================================================

Servo pistonServo;
bool isExtended = false;

bool isInputActive() {
  int raw = digitalRead(CONTROL_PIN);
  return CONTROL_ACTIVE_LOW ? (raw == LOW) : (raw == HIGH);
}

void setup() {
  if (USE_TOGGLE_BUTTON == USE_LEVEL_SWITCH) {
    while (true) {
      delay(1000);
    }
  }

  pinMode(CONTROL_PIN, CONTROL_ACTIVE_LOW ? INPUT_PULLUP : INPUT_PULLDOWN);

  pistonServo.attach(SERVO_PIN);
  pistonServo.write(RETRACTED_ANGLE);
}

void loop() {
  if (USE_TOGGLE_BUTTON) {
    if (isInputActive()) {
      delay(DEBOUNCE_MS);

      if (isInputActive()) {
        isExtended = !isExtended;

        if (isExtended) {
          pistonServo.write(EXTENDED_ANGLE);
        } else {
          pistonServo.write(RETRACTED_ANGLE);
        }

        // Wait for release to avoid retriggering
        while (isInputActive()) {
          delay(10);
        }
      }
    }
  } else {
    bool active = isInputActive();
    bool targetExtended = SWITCH_ACTIVE_MEANS_EXTENDED ? active : !active;

    if (targetExtended != isExtended) {
      isExtended = targetExtended;
      if (isExtended) {
        pistonServo.write(EXTENDED_ANGLE);
      } else {
        pistonServo.write(RETRACTED_ANGLE);
      }
    }
  }

  delay(10);
}
