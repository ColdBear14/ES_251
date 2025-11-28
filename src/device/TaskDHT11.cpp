#include "include/TaskDHT11.h"

DHT_Unified dht(DHTPIN, DHTTYPE);

void TaskDHT11(void *pvParameters)
{
  while (1)
  {
    getValueDHT11();
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}

float getTempeDHT11(){
          sensors_event_t event;
        dht.temperature().getEvent(&event);
        float temperature = event.temperature;
  sendDataSensorData("temperature", String(temperature, 2));
  return temperature;
}

float getHumDHT11(){
          sensors_event_t event;
        dht.humidity().getEvent(&event);
        float humidity = event.relative_humidity;
  sendDataSensorData("humidity", String(humidity, 2));
  return humidity;
}

void getValueDHT11()
{
          sensors_event_t event;
        dht.temperature().getEvent(&event);
        float temperature = event.temperature;
        dht.humidity().getEvent(&event);
        float humidity = event.relative_humidity;

        Serial.print("Temperature:" );
        Serial.println(temperature);
        Serial.print("Humidity:" );
        Serial.println(humidity);
        
        sendDataSensorData("temperature", String(temperature, 2));
        sendDataSensorData("humidity", String(humidity, 2));
    
}

void initDHT11()
{   
    dht.begin();
    
    xTaskCreate(
      TaskDHT11,    // Function to implement the task
      "TaskDHT11",  // Name of the task
      4096,       // Stack size in words
      NULL,        // Task input parameter
      1,           // Priority of the task
      NULL         // Task handle
  );
}


