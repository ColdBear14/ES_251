#include "include/TaskSMS.h"

int SMSValue;

void TaskSMS(void *pvParameters)
{
    // This function is a placeholder for SMS task implementation.
    // You can add your SMS handling code here.
    while (1)
    {
        getValueSMS(); // Read the SMS pin value
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

int getValueSMS()
{
    SMSValue = analogRead(SMS_PIN); // Read the SMS pin value
    // publishData("SMS", String(SMSValue)); 
    return SMSValue; // Return the read value
}

void initSMS()
{
    // xTaskCreate(
    //     TaskSMS,      // Function to implement the task
    //     "TaskSMS",    // Name of the task
    //     2048,         // Stack size in words
    //     NULL,         // Task input parameter
    //     1,            // Priority of the task
    //     NULL          // Task handle
    // );
}