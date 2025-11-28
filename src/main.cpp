#include "globals.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup started");
  InitAP();

  // InitWiFi();

  //InitMQTTTask();

  InitWebServer();

  //initDHT20();
    initDHT11();

  initLCD();
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
