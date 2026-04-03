// Toggle a servo between 0 and 180 degrees each time a button is pressed.
#include <Servo.h>

const int SERVO_PIN = 9;
const int BUTTON_PIN = 2;

const int SERVO_HOME = 0;
const int SERVO_EXTENDED = 180;
const int SERVO_MIN_PULSE_US = 500;
const int SERVO_MAX_PULSE_US = 2500;

const unsigned long DEBOUNCE_MS = 50;
const unsigned long STARTUP_QUIET_MS = 300;
const unsigned long SERVO_ATTACH_SETTLE_MS = 20;
const unsigned long SERVO_MOVE_SETTLE_MS = 650;

Servo deskServo;

bool isExtended = false;
int currentAngle = SERVO_HOME;
int lastButtonReading = HIGH;
int stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;

void moveServoToAngle(int targetAngle) {
	int clampedTarget = constrain(targetAngle, SERVO_HOME, SERVO_EXTENDED);

	// Preload the target start position before attach to reduce first-pulse twitch.
	deskServo.write(currentAngle);

	if (!deskServo.attached()) {
		// Calibrate pulse range so 0..180 maps closer to full mechanical travel.
		deskServo.attach(SERVO_PIN, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US);
		delay(SERVO_ATTACH_SETTLE_MS);
	}

	deskServo.write(clampedTarget);
	delay(SERVO_MOVE_SETTLE_MS);
	deskServo.detach();

	currentAngle = clampedTarget;
}

void setup() {
	pinMode(BUTTON_PIN, INPUT_PULLUP);

	// Keep PWM off at startup to avoid the common boot twitch.
	deskServo.detach();
	delay(STARTUP_QUIET_MS);
}

void loop() {
	int reading = digitalRead(BUTTON_PIN);

	if (reading != lastButtonReading) {
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
		if (reading != stableButtonState) {
			stableButtonState = reading;

			// Button press event (wired from pin to GND with INPUT_PULLUP).
			if (stableButtonState == LOW) {
				isExtended = !isExtended;
				moveServoToAngle(isExtended ? SERVO_EXTENDED : SERVO_HOME);
			}
		}
	}

	lastButtonReading = reading;
}
