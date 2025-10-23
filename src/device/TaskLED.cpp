#include "include/TaskLED.h"


void TaskLED(void *pvParameters)
{

    while (1)
    {
        if(output1State == "on") {
            digitalWrite(LED_PIN, HIGH); // Bật LED
        } else {
            digitalWrite(LED_PIN, LOW); // Tắt LED
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Đợi 1 giây
    }
}

void initLED() {

  // Initialize GPIO pins
  pinMode(LED_PIN, OUTPUT);
  
  // Set initial states
  digitalWrite(LED_PIN, LOW);

    xTaskCreate(
    TaskLED,    // Function to implement the task
    "TaskLED",  // Name of the task
    4096,       // Stack size in words
    NULL,        // Task input parameter
    1,           // Priority of the task
    NULL         // Task handle
    );
}