/*
 * I2CServoDriver.h
 *
 *  Created on: 25. 4. 2016
 *      Author: pbures
 */

#ifndef I2CSERVODRIVER_H_
#define I2CSERVODRIVER_H_

#include <Wire.h>

class I2CServoDriver {

public:
	I2CServoDriver(uint8_t I2CAddress);
	void turn(uint16_t degrees);
private:
	uint8_t address;
};

#endif /* I2CSERVODRIVER_H_ */
