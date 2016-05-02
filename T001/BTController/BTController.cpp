/*
 * BTController.cpp
 *
 *  Created on: 25. 4. 2016
 *      Author: pbures
 */

#include "BTController.h"

#ifndef BLUETOOTHCONTROL_H
#define BLUETOOTHCONTROL_H

//#define BTCONTROL_DEBUG
#define BTMODULE_BAUDRATE 9600

#include <SoftwareSerial.h>
#include <Servo.h>

char BTController::valueNames[NUM_OF_VALUES] = { 'b', 'x', 'y', 'z' };

#define IS_VALID_AXIS(x) ((x >= 0) && (x < NUM_OF_VALUES))
#define IS_VALID_VALUE(x) ((x >= '0') && (x <= '9'))

/*
 * Public Methods
 */
int8_t BTController::getValue(BTControllerAxe axe) {
	if (IS_VALID_AXIS(axe) && (counter > 0))
		return values[axe];

	return INT8_MAX;
}

BTControllerAngles& BTController::getAngles() {

	int8_t x, y, z;
	double rho, phi;

	/*
	 * The X and Y from the BlueTooth joystick controller are swapped!
	 * https://play.google.com/store/apps/details?id=com.heightdev.arduinobtjoystick&hl=pt
	 */
	x = getValue(Y);
	y = getValue(X);
	z = getValue(Z);

	if (x == 0 && y == 0 && z == 0) { //Are we in space with zero gravity? Better return last known value.
		return angles;
	}

	/*
	 * Rho = arctan ( x / sqrt (y^2 + z^2) ).
	 * This shows how much to turn.
	 */
	if (x == 0) {
		angles.turn = 0;
	} else {
		rho = (y * y) + (z * z);
		if (rho == 0) { //divisor goes to infininity -> rho is 90 or -90;
			angles.turn = (x > 0) ? 90 : -90;
		} else {
			rho = x / sqrt(rho);
			rho = atan(rho);
			angles.turn = rho * 180 / M_PI;
		}
	}

	/*
	 * Phi = arctan ( y / sqrt(x^2 + z^2) )
	 * This shows how much to drive the motor.
	 */
	if (y == 0) {
		angles.forward = 0;
	} else {
		phi = (x * x) + (z * z);
		if (phi == 0) { //divisor goes to infinity -> phi is 90 or -90.
			angles.forward = (y > 0) ? 90 : -90;
		} else {
			phi = (double)y / sqrt(phi);
			phi = atan(phi);
			angles.forward = phi * 180 / M_PI;
		}

	}
	return angles;
}

boolean BTController::parseValues() {

	boolean ret = false;

	while (bluetooth.available()) {
		char c = bluetooth.read();

#ifdef BTCONTROL_DEBUG
		Serial.print(c);
#endif

		switch (state) {

		case START:
			if (c == '#')
				state = DELIMITER;
			break;

		case DELIMITER:
			if (c == '#')
				state = AXIS_ID;
			break;

		case AXIS_ID:
			uint8_t ai;
			if ((ai = axisNameToIndex(c)) < NUM_OF_VALUES) {
				axisId = ai;
				valuePowerOfTen[axisId] = 0;
				valueSign[axisId] = 1;
				state = EQUAL_SIGN;
			}
			break;

		case EQUAL_SIGN:
			if (c == '=')
				state = VALUE;
			break;

		case VALUE:
			if (!IS_VALID_AXIS(axisId))
				break;

			if ((c == '-') && valuePowerOfTen[axisId] == 0) {
				valueSign[axisId] = -1;
			} else if (IS_VALID_VALUE(c)) {

				char val = (char) c - 0x30;

				if (valuePowerOfTen[axisId] == 0) {
					values[axisId] = val;
				} else {
					values[axisId] = values[axisId]
							* pow10(valuePowerOfTen[axisId]) + val;
				}
				valuePowerOfTen[axisId]++;
			} else if (c == '#') {
				values[axisId] *= valueSign[axisId];
				foundValues[axisId] = true;
				state = AXIS_ID;
			}

			if (hasAllValues()) {
				if ((counter == 0) || valueChanged())
					ret = true;
				clearValues();
				state = DELIMITER;
				counter++;

#ifdef BTCONTROL_DEBUG
				for (int i = 0; i < NUM_OF_VALUES; i++) {
					Serial.print("  *");
					Serial.print(values[i]);
				}
				Serial.println("*");
#endif
			}
			break; /* End of case VALUE */
		}
	}

	return ret;
}

BTController::BTController(int rx, int tx) :
		bluetooth(rx, tx) {
	reset();
	bluetooth.begin(BTMODULE_BAUDRATE);
}

void BTController::reset() {
	axisId = UINT8_MAX;
	clearValues();
	state = START;
	counter = 0;
}

uint8_t BTController::axisNameToIndex(char c) {
	for (int i = 0; i < NUM_OF_VALUES; i++)
		if (valueNames[i] == c)
			return i;

	return UINT8_MAX;
}

inline boolean BTController::hasAllValues() {
	for (int i = 0; i < NUM_OF_VALUES; i++)
		if (!foundValues[i])
			return false;
	return true;
}

inline void BTController::clearValues() {
	for (int i = 0; i < NUM_OF_VALUES; i++) {
		foundValues[i] = false;
		valuePowerOfTen[i] = 0;
		valueSign[i] = 0;
		previousValues[i] = values[i];
	}

	angles.forward = 0;
	angles.turn = 0;
}

inline uint16_t BTController::pow10(uint8_t pow) {
	uint16_t ret = 1;
	for (int i = 0; i < pow; i++)
		ret = ret * 10;
	return ret;
}

boolean BTController::valueChanged() {
	for (int i = 0; i < NUM_OF_VALUES; i++) {
		if (previousValues[i] != values[i])
			return true;
	}
	return false;
}

void BTController::print() {
	Serial.print(getCounter());
	Serial.print(" :");

	for (int i = 0; i < NUM_OF_VALUES; i++) {
		Serial.print(valueNames[i]);
		Serial.print("=");
		Serial.print(values[i]);
		Serial.print(",");
	}

	BTControllerAngles& a = getAngles();
	Serial.print(" T: ");
	Serial.print(a.turn);
	Serial.print(" D: ");
	Serial.println(a.forward);
}

#endif

