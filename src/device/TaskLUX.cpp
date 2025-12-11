#include "include/TaskLUX.h"

void LuxTask(void *pvParameters) {
  while (true) {
      if(WiFi.status() == WL_CONNECTED) { // Chỉ đọc sensor khi WiFi đã kết nối
          unsigned long now = millis();
              if (now - sensors[2].lastReadTime >= sensors[2].period * 1000) {
                  sensors[2].value = getLux();
                  sensors[2].lastReadTime = now;
                  Serial.printf("Sensor ID: %d, Name: %s, Value: %.2f\n", sensors[2].id, sensors[2].name, sensors[2].value, sensors[2].period);
              }
          }
      vTaskDelay(100 / portTICK_PERIOD_MS); // kiểm tra mỗi 100ms

        }
    }

float getLux() {
    float luxValue = analogRead(LUX_PIN); 
    // float luxValue = 50 + (rand() % (75 - 50 + 1));
    sendDataSensorData(String(sensors[2].id), sensors[2].name, String(luxValue, 2), String(sensors[2].period));
    return luxValue; // Đọc giá trị từ cảm biến ánh sáng
}

void initLUX()
{
    xTaskCreate(
      LuxTask,    // Function to implement the task
      "LuxTask",  // Name of the task
      4000,       // Stack size in words
      NULL,        // Task input parameter
      2,           // Priority of the task
      NULL         // Task handle
  );
}