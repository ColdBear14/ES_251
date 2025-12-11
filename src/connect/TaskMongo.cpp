#include "include/TaskMongo.h"

const char* addServer = "http://10.28.128.146:3000/addData";

const char* getServer = "http://10.28.128.146:3000/getData";

const char* getServer10 = "http://10.28.128.146:3000/getData10";


QueueHandle_t mongoQueue;

unsigned long lastSyncTime = 0;
const unsigned long SYNC_INTERVAL = 5000;

void getHistoryFromMongoDB(const String& collection, const String& sensorId) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        String url = String(getServer10) + "?collection=" + collection;
        if (sensorId != "") {
            url += "&sensorId=" + sensorId;
        }
        http.begin(url);
        
        int httpResponseCode = http.GET();

        if (httpResponseCode > 0) {
            String payload = http.getString();

            sendWebSocketMessage(payload);

        } else {
            Serial.printf("[MONGO] GET Failed: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
    }
}

void getDataFromMongoDB(const String& collection, const String& sensorId) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        String url = String(getServer) + "?collection=" + collection;
        if (sensorId != "") {
            url += "&sensorId=" + sensorId;
        }


        http.begin(url);
        
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            String payload = http.getString();

            sendWebSocketMessage(payload);

        } else {
            Serial.printf("[MONGO] GET Failed: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
    }
}

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

void initMongoTask() {
    mongoQueue = xQueueCreate(10, sizeof(String*));

    xTaskCreate(taskProcessMongoDB, "MongoTask", 12000, NULL, 1, NULL);
}

// Hàm hỗ trợ đẩy dữ liệu vào Queue
void sendToQueue(String* payload) {
    if (mongoQueue != NULL) {
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


