#include "include/TaskDHT20.h"

DHT20 dht20;

void TaskDHT20(void *pvParameters)
{
  while (1)
  {
    getValueDHT20();
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

float getTempeDHT20(){
  dht20.read();
  return dht20.getTemperature();
}

float getHumDHT20(){
  dht20.read();
  return dht20.getHumidity();
}

void getValueDHT20()
{
    if (dht20.read() == DHT20_OK && WiFi.status() == WL_CONNECTED)
    {
        float temperature = dht20.getTemperature();
        float humidity = dht20.getHumidity();
        Serial.print("Temperature:" );
        Serial.println(temperature);
        Serial.print("Humidity:" );
        Serial.println(humidity);
        
        // sendDataSensorData("temperature", String(temperature, 2));
        // sendDataSensorData("humidity", String(humidity, 2));
    }
}

void initDHT20()
{
    Wire.begin(MY_SCL, MY_SDA);
    dht20.begin();
    
  //   xTaskCreate(
  //     TaskDHT20,    // Function to implement the task
  //     "TaskDHT20",  // Name of the task
  //     4096,       // Stack size in words
  //     NULL,        // Task input parameter
  //     1,           // Priority of the task
  //     NULL         // Task handle
  // );
}


