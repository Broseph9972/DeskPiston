from machine import Pin, PWM
from time import sleep_ms

# ===================== USER SETTINGS =====================
# Choose ONE control style by setting True/False below.
USE_TOGGLE_BUTTON = True   # Press once -> extend, press again -> retract
USE_LEVEL_SWITCH = False   # Lever/slide switch: one position extend, other retract

# Pins (change these to whatever GPIO you want to use)
CONTROL_PIN = 15
SERVO_PIN = 16

# Input polarity
CONTROL_ACTIVE_LOW = True  # True for switch/button wired from GPIO to GND (with pull-up)
# Engineering default: one control GPIO + one shared GND is standard for button input.
# 3-pin SPDT slider can still be used; this code reads a single logic state on CONTROL_PIN.

# Only used in lever/slide switch mode:
# True  -> active position means extended
# False -> active position means retracted
SWITCH_ACTIVE_MEANS_EXTENDED = True

# Servo positions
EXTENDED_ANGLE = 90
RETRACTED_ANGLE = 0

# Debounce timing
DEBOUNCE_MS = 25
# ========================================================


class SimpleServo:
    def __init__(self, pin, min_us=500, max_us=2500, freq=50):
        self.pwm = PWM(Pin(pin))
        self.pwm.freq(freq)
        self.min_us = min_us
        self.max_us = max_us
        self.period_us = int(1_000_000 / freq)

    def write_angle(self, angle):
        angle = max(0, min(180, angle))
        pulse_us = self.min_us + (self.max_us - self.min_us) * (angle / 180)
        duty_u16 = int((pulse_us / self.period_us) * 65535)
        self.pwm.duty_u16(duty_u16)


if USE_TOGGLE_BUTTON == USE_LEVEL_SWITCH:
    raise ValueError("Set exactly one of USE_TOGGLE_BUTTON or USE_LEVEL_SWITCH to True.")

pull_mode = Pin.PULL_UP if CONTROL_ACTIVE_LOW else Pin.PULL_DOWN
control_input = Pin(CONTROL_PIN, Pin.IN, pull_mode)
servo = SimpleServo(SERVO_PIN)

is_extended = False
servo.write_angle(RETRACTED_ANGLE)


def is_input_active():
    if CONTROL_ACTIVE_LOW:
        return control_input.value() == 0
    return control_input.value() == 1

while True:
    if USE_TOGGLE_BUTTON:
        if is_input_active():
            sleep_ms(DEBOUNCE_MS)
            if is_input_active():
                is_extended = not is_extended

                if is_extended:
                    servo.write_angle(EXTENDED_ANGLE)
                else:
                    servo.write_angle(RETRACTED_ANGLE)

                # Wait for release to avoid retriggering
                while is_input_active():
                    sleep_ms(10)

    else:
        active = is_input_active()
        target_extended = active if SWITCH_ACTIVE_MEANS_EXTENDED else not active

        if target_extended != is_extended:
            is_extended = target_extended
            if is_extended:
                servo.write_angle(EXTENDED_ANGLE)
            else:
                servo.write_angle(RETRACTED_ANGLE)

    sleep_ms(10)
