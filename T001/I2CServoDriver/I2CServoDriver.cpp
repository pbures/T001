/*
 * I2CServoDriver.cpp
 *
 *  Created on: 25. 4. 2016
 *      Author: pbures
 */

#include "../I2CServoDriver/I2CServoDriver.h"


I2CServoDriver::I2CServoDriver(uint8_t I2CAddress) {
	address = I2CAddress;
	Wire.begin();
}

/*
 * Sends 16bit number, less significant byte first.
 */
void I2CServoDriver::turn(uint16_t degrees) {
	uint16_t deg = degrees + 90;
	Wire.beginTransmission(address);
	Wire.write((uint8_t) (deg & 0x00FF));
	Wire.write(deg >> 8);
	Wire.endTransmission();
}
