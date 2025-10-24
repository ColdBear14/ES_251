#include "include/TaskHC_SR04.h"

float getDistanceHC_SR04()
{
    unsigned long duration; // biến đo thời gian
    float distance;           // biến lưu khoảng cách
    
    /* Phát xung từ chân trig */
    digitalWrite(HC_SR04_TRIG_PIN,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(HC_SR04_TRIG_PIN,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(HC_SR04_TRIG_PIN,0);   // tắt chân trig

    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(HC_SR04_ECHO_PIN,HIGH);  
    // Tính khoảng cách đến vật.
    distance = float(duration/2/29.412);
    // publishData("DIS", String(distance));
    return distance;
}

void TaskHC_SR04(void *pvParameters)
{
    while (1)
    {
        getDistanceHC_SR04();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void initHC_SR04()
{
    pinMode(HC_SR04_TRIG_PIN,OUTPUT);   // chân trig sẽ phát tín hiệu
    pinMode(HC_SR04_ECHO_PIN,INPUT); 

    // xTaskCreate(
    //     TaskHC_SR04,         // Function to implement the task
    //     "TaskHC_SR04",      // Name of the task
    //     2048,           // Stack size in words
    //     NULL,           // Task input parameter
    //     1,              // Priority of the task
    //     NULL            // Task handle
    // );
}


