/*
 * MotorDriver.h
 *
 *  Created on: 3. 5. 2016
 *      Author: pbures
 */

#ifndef MOTORDRIVER_MOTORDRIVER_H_
#define MOTORDRIVER_MOTORDRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

class MotorDriver {
public:
	MotorDriver();
	void init();

	/* Accepts values between -5 to 5 */
	void drive(int8_t speed);

private:
	void initTimer0();

};

#endif /* MOTORDRIVER_MOTORDRIVER_H_ */
