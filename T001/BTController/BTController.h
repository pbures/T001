/*
 * BTController.h
 *
 *  Created on: 25. 4. 2016
 *      Author: pbures
 */

#ifndef BTCONTROLLER_H_
#define BTCONTROLLER_H_
#include <Arduino.h>
#include <SoftwareSerial.h>

#define NUM_OF_VALUES 4

enum BTControllerState {
	DELIMITER, AXIS_ID, EQUAL_SIGN, VALUE, START
};

enum BTControllerAxe {
	B = 0, X = 1, Y = 2, Z = 3
};

struct BTControllerAngles {
	int8_t forward = 0;
	int8_t turn = 0;
};

class BTController {

public:
	BTController(int rx, int tx);

	/*
	 * Iterate over the BlueTooth buffer and read in all the values. Returns true
	 * if new values were detected. Each parsed n-tuple of values increments counter.
	 * The 16bit counter can be accessed by getCounter method.
	 */
	boolean parseValues();

	/*
	 * Clears the counter and puts the parsing state machine on start.
	 */
	void reset();

	/*
	 * Return last known parsed value for given axe, or INT8_MAX in case of error.
	 * The error may be either wrong axis specification, or the requested value
	 * has never been read yet.
	 */
	int8_t getValue(BTControllerAxe axe);

	/*
	 * Returns the current counter value.
	 */
	uint16_t getCounter() const {
		return counter;
	}

	/*
	 * Returns the reference to the BTControllerAngles containing the angle and drive (in degrees)
	 * how the BTController is tilted forward/backwards or left/right.
	 *
	 * The method does not call the parseValues() to parse latest buffered values
	 * coming from BlueTooth, be sure you are calling it before.
	 */
	BTControllerAngles& getAngles();

	/*
	 * Prints the values nicely formatted to Serial link.
	 */
	void print();

private:
	SoftwareSerial bluetooth;

	/* Store any parsed valid value here */
	int8_t previousValues[NUM_OF_VALUES];
	int8_t values[NUM_OF_VALUES];
	boolean foundValues[NUM_OF_VALUES];
	uint8_t valuePowerOfTen[NUM_OF_VALUES];
	int8_t valueSign[NUM_OF_VALUES];
	BTControllerAngles angles;
	static char valueNames[NUM_OF_VALUES];


	/* State Machine Controls */
	BTControllerState state;
	int axisId;
	uint16_t counter;

	inline boolean hasAllValues();
	inline void clearValues();
	uint8_t axisNameToIndex(char c);
	uint16_t pow10(uint8_t pow);
	boolean valueChanged();

};

#endif /* BTCONTROLLER_H_ */
