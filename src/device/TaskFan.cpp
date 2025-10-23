#include "include/TaskFan.h"

void TaskFan(void *pvParameters)
{
    pinMode(FAN_PIN, OUTPUT); // Set the fan pin as output
    bool state = false;
    while (1)
    {
        if(output3State == "on"){
            state = !state;
            digitalWrite(FAN_PIN, state ? HIGH : LOW); // Toggle the relay state
        }
        else{
            digitalWrite(FAN_PIN, LOW);  
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1
    }
}

void initFan()
{
//     xTaskCreate(
//       TaskFan,    // Function to implement the task
//       "TaskFan",  // Name of the task
//       4096,       // Stack size in words
//       NULL,        // Task input parameter
//       1,           // Priority of the task
//       NULL         // Task handle
//   );
}