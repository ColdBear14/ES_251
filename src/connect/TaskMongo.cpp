#include "include/TaskMongo.h"

const char* addServer = "http://192.168.4.2:3000/addData";

// Tạo hàng đợi để chứa các String JSON cần gửi
QueueHandle_t mongoQueue;

// Task này sẽ chạy liên tục, chờ có dữ liệu trong Queue thì lấy ra gửi
void taskProcessMongoDB(void *pvParameters) {
    String *receivedData;
    HTTPClient http;

    while (true) {
        // Chờ dữ liệu từ Queue (chờ vô hạn portMAX_DELAY)
        if (xQueueReceive(mongoQueue, &receivedData, portMAX_DELAY) == pdPASS) {
            
            if (WiFi.status() == WL_CONNECTED) { // Kiểm tra WiFi trước khi gửi
                http.begin(addServer);
                http.addHeader("Content-Type", "application/json");

                int httpResponseCode = http.POST(*receivedData);

                if (httpResponseCode > 0) {
                    // Serial.println(http.getString()); // Debug nếu cần
                } else {
                    Serial.printf("Error sending data: %s\n", http.errorToString(httpResponseCode).c_str());
                }
                http.end();
            } else {
                Serial.println("WiFi Disconnected, skipping Mongo upload");
            }

            // QUAN TRỌNG: Giải phóng bộ nhớ đã cấp phát bên ngoài
            delete receivedData; 
        }
    }
}

// Hàm khởi tạo cần gọi trong setup() hoặc main.cpp
void initMongoTask() {
    // Tạo Queue chứa được 10 con trỏ String
    mongoQueue = xQueueCreate(10, sizeof(String*));

    // Tạo 1 Task duy nhất để xử lý việc gửi
    xTaskCreate(taskProcessMongoDB, "MongoTask", 6144, NULL, 1, NULL);
}

// Hàm hỗ trợ đẩy dữ liệu vào Queue
void sendToQueue(String* payload) {
    if (mongoQueue != NULL) {
        // Gửi con trỏ vào Queue, nếu Queue đầy thì không đợi (0)
        if (xQueueSend(mongoQueue, &payload, 0) != pdPASS) {
            Serial.println("Queue full! Dropping data.");
            delete payload; // Nếu không gửi được thì phải xóa ngay để tránh Memory Leak
        }
    } else {
        delete payload;
    }
}

void sendDataActionHistory(const String& name, const String& action) {
    String* data = new String;
    *data += "{\"collection\":\"action\",\"name\":\"";
    *data += name;
    *data += "\",\"action\":\""; 
    *data += action; 
    *data += "\"}";
    sendToQueue(data);
}

void sendDataSensorData(const String& id, const String& sensorName, const String& sensorValue, const String& period) {
    String* data = new String;
    *data += "{\"collection\":\"sensor\",\"sensorId\":\"";
    *data += id;
    *data += "\",\"name\":\""; 
    *data += sensorName;
    *data += "\",\"value\":\""; 
    *data += sensorValue; 
    *data += "\",\"period\":\""; 
    *data += period;
    *data += "\"}";
    sendToQueue(data);
}

void sendDataSettings(const int& id, const String& name, const int& period) {
    String* data = new String;
    *data += "{\"collection\":\"settings\",\"id\":\"";
    *data += id;
    *data += "\",\"name\":\""; 
    *data += name; 
    *data += "\",\"period\":\"";
    *data += period;
    *data += "\"}";
    sendToQueue(data);
}