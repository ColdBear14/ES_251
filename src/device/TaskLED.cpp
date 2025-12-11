#include "include/TaskLED.h"

// Khai báo biến Handle để quản lý Task
TaskHandle_t xTaskLEDHandle = NULL;

void TaskLED(void *pvParameters)
{
    while (1)
    {
        // 1. CHỜ TÍN HIỆU:
        // Task sẽ ngủ yên tại đây (Block) cho đến khi nhận được thông báo.
        // portMAX_DELAY nghĩa là chờ mãi mãi.
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); 

        // 2. NHẬN ĐƯỢC TÍN HIỆU -> BẬT LED
        digitalWrite(LED_PIN, HIGH);
        
        // 3. ĐỢI 1 GIÂY
        vTaskDelay(1000 / portTICK_PERIOD_MS); 

        // 4. TẮT LED
        digitalWrite(LED_PIN, LOW);
        
        // Sau đó vòng lặp quay lại bước 1 và tiếp tục ngủ chờ lần gọi tiếp theo
    }
}

// Hàm này dùng để gọi từ bất cứ đâu (trong loop, task khác, ngắt, v.v.)
void triggerLedBlink() {
    if (xTaskLEDHandle != NULL) {
        // Gửi tín hiệu đánh thức TaskLED
        xTaskNotifyGive(xTaskLEDHandle);
    }
}

void initLED() {
    // Initialize GPIO pins
    pinMode(LED_PIN, OUTPUT);
    
    // Set initial states
    digitalWrite(LED_PIN, LOW);

    xTaskCreate(
        TaskLED,        // Function to implement the task
        "TaskLED",      // Name of the task
        2048,           // Stack size (Nên tăng lên ~2048 để tránh Stack Overflow trên ESP32)
        NULL,           // Task input parameter
        2,              // Priority of the task
        &xTaskLEDHandle // QUAN TRỌNG: Lưu tham chiếu vào biến Handle
    );
}