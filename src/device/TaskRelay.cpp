#include "include/TaskRelay.h"

void TaskRelay(void *pvParameters)
{
    bool state = false;
    while (1)
    {
        if(output3State == "on"){
            state = !state;
            digitalWrite(Relay_PIN, state ? HIGH : LOW); // Toggle the relay state
        }
        else{
            digitalWrite(Relay_PIN, LOW);  
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Wait for 1
    }
}

void initRelay()
{
    pinMode(Relay_PIN, OUTPUT);

    // xTaskCreate(
    //     TaskRelay,   // Function to implement the task
    //     "TaskRelay", // Name of the task
    //     2048,        // Stack size in words
    //     NULL,        // Task input parameter
    //     1,           // Priority of the task
    //     NULL         // Task handle
    // );
}