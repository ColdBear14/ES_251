#include "include/TaskLCD.h"

void displayTemperatureAndHumidity(LiquidCrystal_I2C &lcd) {
  for (int i = 0; i < 5; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: " + String(getTempeDHT20()) + " C");
    lcd.setCursor(0, 1);
    lcd.print("Humid: " + String(getHumDHT20()) + "%");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void displayAnalogReadings(LiquidCrystal_I2C &lcd) {
  for (int i = 0; i < 5; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("A0:" + String(analogRead(1)));
    lcd.setCursor(8, 0);
    lcd.print("A1:" + String(analogRead(2)));
    lcd.setCursor(0, 1);
    lcd.print("A2:" + String(analogRead(3)));
    lcd.setCursor(8, 1);
    lcd.print("A3:" + String(analogRead(4)));
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void displayDistance(LiquidCrystal_I2C &lcd) {
  for (int i = 0; i < 5; i++) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance: " + String(getDistanceHC_SR04()) + "cm");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskLCD(void *pvParameters)
{
  LiquidCrystal_I2C lcd(33, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

	lcd.init(MY_SCL, MY_SDA); // initialize the lcd to use user defined I2C pins
	lcd.backlight();

	lcd.setCursor(3,0);
	lcd.print("Hello, world!");
	lcd.setCursor(2,1);
	lcd.print("Time is now");

  while (1)
  {
    displayTemperatureAndHumidity(lcd);
    // displayAnalogReadings(lcd);
    // displayDistance(lcd);
  }
}

void initLCD()
{
  Wire.begin(MY_SCL, MY_SDA);
  xTaskCreate(
    TaskLCD,      // Function to implement the task
    "TaskLCD",   // Name of the task
    4096,        // Stack size in words
    NULL,        // Task input parameter
    1,           // Priority of the task
    NULL         // Task handle
  );
}