/*
 * Display.h
 *
 *  Created on: 25. 4. 2016
 *      Author: pbures
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <LiquidCrystal_I2C.h>

#define LCD_EN 2
#define LCD_RW 1
#define LCD_RS 0
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define LCD_BACKLIGHT_PIN 3
#define LCD_BACKLIGHT_POL POSITIVE


class LCDDisplay : public LiquidCrystal_I2C {

private:
	boolean initialized = false;


public:
	//static LiquidCrystal_I2C& getLCD();
	LCDDisplay();
	void init();
};

#endif /* DISPLAY_H_ */
