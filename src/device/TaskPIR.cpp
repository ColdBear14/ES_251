#include "include/TaskPIR.h"


void TaskPIR(void *pvParameters)
{
    while (1)
    {
        getValuePIR();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void getValuePIR()
{
    // Simulate PIR sensor value retrieval
    int pirValue = digitalRead(PIR_SENSOR_PIN); // Assuming PIR sensor is connected to GPIO 2
    Serial.println("PIR Sensor Value: " + String(pirValue));

    std::stringstream ss;
    ss << "PIR: " << pirValue;
    publishData("PIR", String(ss.str().c_str()));
}

void initPIR()
{
    pinMode(PIR_SENSOR_PIN, INPUT); // Assuming PIR sensor is connected to GPIO 2
    // xTaskCreate(
    //     TaskPIR,         // Function to implement the task
    //     "TaskPIR",      // Name of the task
    //     2048,           // Stack size in words
    //     NULL,           // Task input parameter
    //     1,              // Priority of the task
    //     NULL            // Task handle
    // );
}

