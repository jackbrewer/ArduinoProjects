/*

Treat Dispenser

This circuit is designed to train my pet rats to push a button to receive a treat.
The button can only be pressed when a LED is lit.
Once the button has been pressed, a servo rotates to deliver a treat, then resets.
The LED then turns off for a set amount of time, before turning back on and
reactivating the button.

Circuit:
* Servo attached to pin 9, +5v and ground
* LED attached to pin 3 and ground via resistor
* Pushbutton attached to pin 2, +5v via resistor and ground

Jack Brewer
http://jackbrewer.co.uk

*/

// Define Servo variables and set initial position
#include <Servo.h>
Servo myservo;
const int servoPin =  5;
int pos = 45;

// Define Pushbutton variables
const int buttonPin = 4;

// Define LED variables
const int ledPin = 3;
int ledIntensity = 20;

// Define logic variables
boolean buttonEnabled = false;                                  // Whether the button is accepting presses
boolean isMoving = false;                                       // Whether a treat is currently being dispensed
int buttonState = 0;                                            // Whether the button is currently being pressed
const unsigned long requestInterval = 8000;                     // Time before button re-activates
unsigned long currentTime = millis();                           // Stores the current time
unsigned long lastAttemptTime = currentTime - requestInterval;  // Remembers when the last treat was dispensed


void setup() {

	// Set up pins and move servo to starting position
	pinMode(buttonPin, INPUT);
	pinMode(ledPin, OUTPUT);
	myservo.attach(servoPin);
	myservo.write(pos);

}

void loop(){

	// If enough time has passed
	if (buttonEnabled) {
		checkForPress();     // Listen for button presses
	} else {
		checkRateLimiter();  // Check time again
	}

}

void checkForPress() {

	// Check if button is being pressed
	buttonState = digitalRead(buttonPin);

	// If button is pressed
	if (buttonState == LOW) {

		// Deliver treat and reset
		buttonEnabled = false;
		analogWrite(ledPin, 0);
		deliverTreat();

	}
}

void checkRateLimiter() {

	unsigned long currentTime = millis();

	// If enough time has passed
	if (currentTime - lastAttemptTime >= requestInterval) {

		// Activate Button and LED
		buttonEnabled = true;
		analogWrite(ledPin, ledIntensity);
		lastAttemptTime = currentTime;

	}
}

void deliverTreat(){

	if (!isMoving) {

		isMoving = true;

		for(pos = 45; pos < 135; pos += 1) {
			myservo.write(pos);
			delay(5);
		}
		delay(500);
		for(pos = 135; pos>=45; pos-=1) {
			myservo.write(pos);
			delay(10);
		}

		isMoving = false;

		// Reset the timer to start again from the time the treat is dispensed
		lastAttemptTime = millis();

	}
}