#include "globals.h"

void setup()
{
  Serial.begin(115200);
  InitAP();

  // InitWifi();

  // InitMQTTTask();

  initMongoTask();

  InitWebServer();

  initDHT20();
  initLUX();

  // initLCD();
  // initLED();

  // initRGB();
  // initRelay();
  // initFan();

  // initPIR();

  // initHC_SR04();
  // initSMS();
  

}

void loop()
{


}
