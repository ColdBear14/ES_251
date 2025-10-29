#include "globals.h"

void setup()
{
  Serial.begin(115200);
  InitAP();

  // InitWiFi();

  InitMQTTTask();

  InitWebServer();

  initDHT20();
  // initLCD();
  // initLED();

  // initRGB();
  // initRelay();
  // initFan();

  // initPIR();

  // initLUX();
  // initHC_SR04();
  // initSMS();
  

}

void loop()
{
  if (!Wifi_reconnect())
  {
    return;
  }


}
