#include "include/TaskMongo.h"

const char* addServer = "http://192.168.4.2:3000/addData";

void sendDataToMongoDBTask(void* parameter) {

    String data = *(String*)parameter;
    HTTPClient http;
    http.begin(addServer);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(data);

    if (httpResponseCode > 0) {
        String response = http.getString();
    } else {
        Serial.printf("Error sending data: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
    vTaskDelete(NULL); // Delete the task once done
}

void sendDataActionHistory(const String& name, const String& action) {
    String* data = new String;
    *data += "{\"collection\":\"action\",\"name\":\"";
    *data += name;
    *data += "\",\"action\":\""; 
    *data += action; 
    *data += "\"}";
    xTaskCreate(sendDataToMongoDBTask, "SendDataActionHistory", 4096, data, 2, NULL);
}

void sendDataSensorData(const String& sensorName, const String& sensorValue) {
    String* data = new String;
    *data += "{\"collection\":\"sensor\",\"name\":\"";
    *data += sensorName;
    *data += "\",\"value\":\""; 
    *data += sensorValue; 
    *data += "\"}";
    xTaskCreate(sendDataToMongoDBTask, "SendDataSensorData", 4096, data, 2, NULL);
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
    xTaskCreate(sendDataToMongoDBTask, "sendDataSettings", 4096, data, 2, NULL);
}
