/*
 * MotorDriver.cpp
 *
 *  Created on: 3. 5. 2016
 *      Author: pbures
 */

#include "MotorDriver.h"

MotorDriver::MotorDriver() {

}

void MotorDriver::init() {
	/* Set output on PIN6 (PWM), PIN4 (Driver EN1),  PIN5 (Driver EN2) */
	DDRD |= ((1 << PD6) | (1 << PD4) | (1 << PD5));
	initTimer0();
}

void MotorDriver::drive(int8_t speed) {
	/*
	 * TODO: set the proper value based on speed! Check data sheet.
	 */

	/* Convert approx range -45 to 45 into  -3, -2, -1, 0, 1, 2, 3 speeds */
	int16_t s = speed;
	if (s > 45) s = 45;
	if (s < -45) s = -45;

	s = round(s/15); //values: -3,-2,-1,0,1,2,3

	if (s == 0) {
		PORTD &= ~((1<<PD4) | (1<<PD5));
	} else if (s > 0) {
		PORTD &= ~(1<<PD5);
		PORTD |= (1<<PD4);
	} else {
		PORTD &= ~(1<<PD4);
		PORTD |= (1<<PD5);
	}

	s = (s < 0)?-1*s:s;
	OCR0A = s * 85 // (85 = 255/3).
}


void MotorDriver::initTimer0() {
	/*
	 * TODO: Check the values by the data sheet
	 * Set the output to PIN6 == OC0A == PD6.
	 */

	/*
	 * Page 105:
	 * COM0A0==1, COM0A1==1: Clear OC0A on Compare Match when up-counting.
	 * Set OC0A on Compare Match when down-counting.
	 *
	 * Page 106:
	 * WGM00==1, WGM01==1, WGM02==0: Fast PWM, TOP=0xFF, Update OC0A on BOTTOM.
	 */
	TCCR0A |= ((1 << WGM00) | (1 << WGM01) | (1 << COM0A0) | (1 << COM0A1));

	/*
	 * Page 108: Clock prescaler to CS02: /256 or CS00 : /1024.
	 */
	TCCR0B |= (1 << CS02);
}
