#include "globals.h"

void setup()
{
  Serial.begin(115200);
  // InitAP();

  InitWiFi();

  // initMQTT();
  // InitMQTTTask();

  // InitWebServer();

  // InitMail();

  // initDHT20();
  // initLCD();
  // initLED();

  // initRGB();
  // initRelay();
  // initFan();

  // initPIR();

  // initLUX();
  // initHC_SR04();
  // initSMS();
  
  // InitRS485();

}

void loop()
{
  if (!Wifi_reconnect())
  {
    return;
  }


}
