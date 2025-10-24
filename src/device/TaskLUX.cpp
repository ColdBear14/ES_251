#include "include/TaskLUX.h"

void TaskLUX(void *pvParameters)
{

    while (1)
    {
        getValueLux();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void getValueLux() {
    int luxValue = analogRead(LUX_PIN); 
    // publishData("LUX", String(luxValue)); // Giả sử bạn có hàm publishData để gửi dữ liệu lên MQTT
}

float getLux() {
    float luxValue = analogRead(LUX_PIN); 
    return luxValue; // Đọc giá trị từ cảm biến ánh sáng
}

void initLUX()
{
//     xTaskCreate(
//       TaskLUX,    // Function to implement the task
//       "TaskLUX",  // Name of the task
//       4096,       // Stack size in words
//       NULL,        // Task input parameter
//       1,           // Priority of the task
//       NULL         // Task handle
//   );
}