/*
 Name:		CasinoMortale.ino
 Created:	11/29/2021 8:43:35 PM
 Author:	baspa
*/



// the setup function runs once when you press reset or power the board
#include "Button.h"
#include <Servo.h>

Servo servo;
const int servoPin = 9;
const int minPulseWidth = 900;
const int maxPulseWidth = 2100;
const int leftButtonPin = 3;
const int centerButtonPin = 4;
const int rightButtonPin = 5;
const unsigned long debounceDelay = 50;
float destinationServoPosition = 90.0f;
float currentServoPosition = 90;
Bas::Button leftButton{ leftButtonPin, debounceDelay };
Bas::Button centerButton{ centerButtonPin, debounceDelay };
Bas::Button rightButton{ rightButtonPin, debounceDelay };
unsigned long lastTime;
float eyeSpeed = 200.0f;
const unsigned long minEyePause = 500;
const unsigned long maxEyePause = 4000;
unsigned long eyePauseDuration;
unsigned long eyePauseStartTime;
bool isEyeMovementOverRidden = false;

void setup() {
	
	Serial.begin(9600);

	servo.attach(servoPin, minPulseWidth, maxPulseWidth); // This particular servo does not support the default pulse widths of 544 and 2400.
	leftButton.initialize(onLeftButtonDown, onButtonUp);
	centerButton.initialize(onCenterButtonDown, onButtonUp);
	rightButton.initialize(onRightButtonDown, onButtonUp);
	
	servo.write((int)currentServoPosition);
	lastTime = millis();	
}

// the loop function runs over and over again until power down or reset
void loop() {
	leftButton.update();
	centerButton.update();
	rightButton.update();

	moveEye();

	if (!isEyeMovementOverRidden &&
		millis() - eyePauseStartTime > eyePauseDuration &&
		currentServoPosition == destinationServoPosition)
	{
		eyePauseStartTime = millis();
		eyePauseDuration = random(minEyePause, maxEyePause);
		destinationServoPosition = random(0, 180);
	}

	lastTime = millis();
}

void moveEye()
{
	if (currentServoPosition != destinationServoPosition)
	{
		float timeDelta = (millis() - lastTime) / 1000.0f;
		float increment = eyeSpeed * timeDelta;
		if (currentServoPosition < destinationServoPosition)
		{
			currentServoPosition += increment;
		}
		else if (currentServoPosition > destinationServoPosition)
		{
			currentServoPosition -= increment;
		}

		if (abs(currentServoPosition - destinationServoPosition) < 1.0f )			
		{
			currentServoPosition = destinationServoPosition;
		}

		servo.write((int)currentServoPosition);
	}	
}

void onButtonUp()
{
	eyeSpeed = 200;
	isEyeMovementOverRidden = false;
	destinationServoPosition = 90;
	Serial.println("Moving servo to center (90 degrees)");
}

void onLeftButtonDown()
{
	eyeSpeed = 1000;
	isEyeMovementOverRidden = true;
	destinationServoPosition = 0;
	Serial.println("Moving servo to left (0 degrees)");	
}

void onCenterButtonDown()
{
	eyeSpeed = 1000;
	isEyeMovementOverRidden = true;
	destinationServoPosition = 90;
	Serial.println("Moving servo to center (90 degrees)");
}

void onRightButtonDown()
{
	eyeSpeed = 1000;
	isEyeMovementOverRidden = true;
	destinationServoPosition = 180;
	Serial.println("Moving servo to right (180 degrees)");
}
