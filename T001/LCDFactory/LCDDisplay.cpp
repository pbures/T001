/*
 * Display.cpp
 *
 *  Created on: 25. 4. 2016
 *      Author: pbures
 */

#include "LCDDisplay.h"


LCDDisplay::LCDDisplay(): LiquidCrystal_I2C(0x27, LCD_EN, LCD_RW, LCD_RS, LCD_D4, LCD_D5, LCD_D6,
		LCD_D7, LCD_BACKLIGHT_PIN, LCD_BACKLIGHT_POL)
{
}

void LCDDisplay::init() {

	if (initialized)
		return;

	begin(16, 2);
	backlight();
	setCursor(0, 0);

	initialized = true;
}
