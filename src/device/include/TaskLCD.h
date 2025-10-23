#ifndef INC_TASKLCD_H_
#define INC_TASKLCD_H_

#include "globals.h"

extern void displayTemperatureAndHumidity(LiquidCrystal_I2C &lcd);
extern void displayAnalogReadings(LiquidCrystal_I2C &lcd);
extern void displayDistance(LiquidCrystal_I2C &lcd);

extern void TaskLCD(void *pvParameters);

extern void initLCD();

#endif /* INC_TASKLCD_H_ */