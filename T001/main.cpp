#include "Arduino.h"
#include <Wire.h>

#include "I2CServoDriver/I2CServoDriver.h"
#include "BTController/BTController.h"
#include "LCDFactory/LCDDisplay.h"
#include "MotorDriver/MotorDriver.h"

LCDDisplay lcd;
I2CServoDriver servoDriver(0x10);
BTController btController(11, 10);
MotorDriver motorDriver;

void setup() {
	Serial.begin(9600);
	Wire.begin();

	Serial.println("T001 Started.");

	lcd.init();
	lcd.setCursor(0, 0);
	lcd.print("Hello!");
	lcd.setCursor(0, 1);
	lcd.print("T001 Greets you.");

	delay(2);
	lcd.clear();
	lcd.setCursor(0, 0);
	Serial.println("T001 Online...");
}

uint16_t angle = 0;
char deg[3];
char incChar;

void loop() {

	if (btController.parseValues()) {
		btController.print();
		BTControllerAngles& angles = btController.getAngles();

		servoDriver.turn(angles.turn);
		motorDriver.drive(angles.forward);

		lcd.setCursor(0,1);
		lcd.print("T:");
		lcd.print(angles.turn);
		lcd.print(" D:");
		lcd.print(angles.forward);
		lcd.print("  ");
	}

	/*
	 * Debug code, will be removed.

	Serial.println("Enter angle in degrees. Waiting.");
	for (int i = 0; i < 3; i++)
		deg[i] = 0;
	incChar = '0';

	do {
		if (Serial.available() > 0) {
			incChar = Serial.read();
			if (incChar < '0' || incChar > '9')
				continue;
			deg[2] = deg[1];
			deg[1] = deg[0];
			deg[0] = incChar - '0';
			// say what you got:
			Serial.print("I received: ");
			Serial.println(incChar);
			Serial.print(deg[2]);
			Serial.print(deg[1]);
			Serial.println(deg[0]);
		}
	} while (incChar != '\r');

	angle = (deg[2] * 100) + (deg[1] * 10) + deg[0];

	servoDriver.turn(angle);

	Serial.print("Angle: ");
	Serial.print(angle);
	Serial.print(" hex:(");

	lcd.clear();
	lcd.print("Angle:");
	lcd.print(angle);
	lcd.display();

	Serial.print((uint8_t) (angle & 0x00FF), HEX);
	Serial.print(", ");
	Serial.print(angle >> 8, HEX);
	Serial.println(") sent.");
	*/
}
