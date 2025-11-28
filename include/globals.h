#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>


// include libraries
#include <HTTPClient.h>
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "FS.h"
#include <LittleFS.h>
#include <DHT20.h>
#include <PubSubClient.h>
#include "NTPClient.h"
#include <Ultrasonic.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include <ElegantOTA.h>

#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

///
#include "../src/connect/include/TaskWifi.h"
#include "../src/connect/include/TaskMQTT.h"
#include "../src/connect/include/TaskWebServer.h"
#include "../src/connect/include/TaskAP.h"
#include "../src/connect/include/TaskMongo.h"

#include "../src/device/include/TaskDHT20.h"
#include "../src/device/include/TaskDHT11.h"
#include "../src/device/include/TaskPIR.h"
#include "../src/device/include/TaskHC_SR04.h"
#include "../src/device/include/TaskLED.h"
#include "../src/device/include/TaskRGB.h"
#include "../src/device/include/TaskFan.h"
#include "../src/device/include/TaskLUX.h"
#include "../src/device/include/TaskSMS.h"
#include "../src/device/include/TaskLCD.h"
#include "../src/device/include/TaskRelay.h"


#define delay_time 10000

#define PIR_SENSOR_PIN 7 

#define HC_SR04_TRIG_PIN 18
#define HC_SR04_ECHO_PIN 21

#define LUX_PIN 3 
#define SMS_PIN 1 

#define PIN_NEO_PIXEL  6   
#define Relay_PIN 8 
#define FAN_PIN 10 
#define LED_PIN 48

#define MY_SCL 11
#define MY_SDA 12

#define DHTPIN 2 
#define DHTTYPE    DHT11 


struct Sensor {
    int id;
    const char* name;
    int period;
    float value;
    unsigned long lastSentTime;
};

extern Sensor sensors[3];

#endif