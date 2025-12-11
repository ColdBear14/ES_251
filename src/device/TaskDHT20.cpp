#include "include/TaskDHT20.h"

DHT20 dht20;

void TempTask(void *pvParameters) {
  while (true) {
      if(WiFi.status() == WL_CONNECTED) { // Chỉ đọc sensor khi WiFi đã kết nối
          unsigned long now = millis();
              if (now - sensors[0].lastReadTime >= sensors[0].period * 1000) {
                  sensors[0].value = getTempeDHT20();
                  sensors[0].lastReadTime = now;
                  Serial.printf("Sensor ID: %d, Name: %s, Value: %.2f\n", sensors[0].id, sensors[0].name, sensors[0].value, sensors[0].period);
              }
          }
        vTaskDelay(100 / portTICK_PERIOD_MS); // kiểm tra mỗi 100ms
      }
}
void HumidTask(void *pvParameters) {
  while (true) {
      if(WiFi.status() == WL_CONNECTED) { 
          unsigned long now = millis();
              if (now - sensors[1].lastReadTime >= sensors[1].period * 1000) {
                  sensors[1].value = getHumDHT20();
                  sensors[1].lastReadTime = now;
                  Serial.printf("Sensor ID: %d, Name: %s, Value: %.2f\n", sensors[1].id, sensors[1].name, sensors[1].value, sensors[1].period);
              }
          }
        vTaskDelay(100 / portTICK_PERIOD_MS); // kiểm tra mỗi 100ms
    }
}

float getTempeDHT20(){
  dht20.read();
  float temperature = dht20.getTemperature();
  // float temperature = 20.0 + (rand() % 150) / 10.0;
  sendDataSensorData(String(sensors[0].id), sensors[0].name, String(temperature, 2), String(sensors[0].period));
  return temperature;
}

float getHumDHT20(){
  dht20.read();
  float humidity = dht20.getHumidity();
  // float humidity = 40.0 + (rand() % 150) / 10.0;
  sendDataSensorData(String(sensors[1].id), sensors[1].name, String(humidity, 2), String(sensors[1].period));
  return humidity;
}

void initDHT20()
{
    Wire.begin(MY_SCL, MY_SDA);
    dht20.begin();
    
    xTaskCreate(TempTask, "TempTask", 4000, NULL, 2, NULL);

    xTaskCreate(HumidTask, "HumidTask", 4000, NULL, 2, NULL);

  }


