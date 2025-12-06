#include "include/TaskMongo.h"

const char* addServer = "http://10.135.180.108:3000/addData";
const char* getServer = "http://10.135.180.108:3000/getData";

QueueHandle_t mongoQueue;

unsigned long lastSyncTime = 0;
const unsigned long SYNC_INTERVAL = 5000;

void getDataFromMongoDB(const String& collection, const String& sensorId) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        // 1. Tạo URL query parameters
        String url = String(getServer) + "?collection=" + collection;
        if (sensorId != "") {
            url += "&sensorId=" + sensorId;
        }


        http.begin(url);
        
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            String payload = http.getString();

            JsonDocument doc; 
            doc["action"] = "mongo_update";
            
            JsonDocument serverDoc;
            DeserializationError error = deserializeJson(serverDoc, payload);
            
            if (!error) {
                doc["data"] = serverDoc; // Nhúng object/array từ server vào
                
                String wsMessage;
                serializeJson(doc, wsMessage);
                
                sendWebSocketMessage(wsMessage);
                Serial.println("[MONGO] Data forwarded to WebSocket");
            } else {
                Serial.println("[MONGO] Server response not JSON");
            }

        } else {
            Serial.printf("[MONGO] GET Failed: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
    }
}

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
                } else {
                    Serial.printf("Error sending data: %s\n", http.errorToString(httpResponseCode).c_str());
                }
                http.end();
            } else {
                Serial.println("WiFi Disconnected, skipping Mongo upload");
            }

            delete receivedData; 
        }
        if (millis() - lastSyncTime > SYNC_INTERVAL) {
            for(int i = 0; i < 3; i++) {
                String sensorId = String(sensors[i].id);
                getDataFromMongoDB("sensor", sensorId); 
            }            

            
            lastSyncTime = millis();
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


