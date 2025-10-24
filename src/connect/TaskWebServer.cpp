#include "include/TaskWebServer.h"

String output1State = "off";
String output2State = "off";
String output3State = "off";
String output4State = "off";

// Khởi tạo máy chủ HTTP và WebSocket
AsyncWebServer server(8080); // Máy chủ HTTP chạy trên cổng 80
AsyncWebSocket ws("/ws"); // Tạo WebSocket tại endpoint "/ws"

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    }
    else if (type == WS_EVT_DATA)
    {
        String message = String((char *)data);
        parseWebSocketMessage(client, message); // Gọi hàm xử lý tin nhắn
        Serial.printf("WebSocket client #%u sent data: %s\n", client->id(), message.c_str());
  }
}

void parseWebSocketMessage(AsyncWebSocketClient *client, const String &message) {
    // Xử lý tin nhắn từ client xử lý lệnh action
    if (message.startsWith("{\"action\":\"settings\"")) {
        handleSettings(message);
    }
    else if (message.startsWith("{\"action\":\"wifi\"")) {
        // Xử lý cấu hình WiFi
        handleWifiConfig(message);
    }
    else if (message.startsWith("{\"action\":\"mqtt\"")) {
        // Xử lý cấu hình MQTT
        handleMQTT(message);
    }
}

void handleSettings(const String &message){
    JsonDocument doc;

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(doc, message);

    // Extract values from the JSON document
    int sensorID = doc["id"].as<int>();
    const char* name = doc["name"];
    int period = doc["period"].as<int>();

    switch (sensorID)
    {
    case 1: 
        sensors[0].name = name;
        sensors[0].period = period;
        break;
    case 2: 
        sensors[1].name = name;
        sensors[1].period = period;
        break;
    case 3: 
        sensors[2].name = name;
        sensors[2].period = period;
        break;
    default:
        break;
    }

}

void handleMQTT(const String &message) {
    JsonDocument doc;

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(doc, message);

    // Extract values from the JSON document
    const char* username = doc["username"];
    const char* key = doc["key"];

    IO_USERNAME = username; // Lưu username vào biến toàn cục
    IO_KEY = key;           // Lưu key vào biến toàn cục

    MQTT_STATE = 1;
}

void handleWifiConfig(const String &message) {
    JsonDocument doc;

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(doc, message);

    // Extract values from the JSON document
    const char* ssid = doc["ssid"];
    const char* password = doc["password"];

    wifi_ssid = ssid;
    wifi_password = password;

    WIFI_STATE = 1;

    InitWiFi();
}

void initWebServer() {

  if (!LittleFS.begin(true))
  {
      Serial.println("An Error has occurred while mounting LittleFS");
      return;
  }

    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file) {
        Serial.println(file.name());
        file = root.openNextFile();
    }
  ElegantOTA.begin(&server);

  ws.onEvent(onEvent);

  server.addHandler(&ws);
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/wifi.html", "text/html"); });
    server.on("/device", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });
    server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/dashboard.html", "text/html"); });

    server.on("/js/Wifi.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/js/Wifi.js", "application/javascript"); });
    server.on("/js/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/js/script.js", "application/javascript"); });
    server.on("/js/chart.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/js/chart.js", "application/javascript"); });
    server.on("/js/dashboard.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/js/dashboard.js", "application/javascript"); });
            
    server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/css/styles.css", "text/css"); });
    server.on("/css/all.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/css/all.min.css", "text/css"); });

    server.on("/webfonts/fa-solid-900.woff2", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/webfonts/fa-solid-900.woff2", "font/woff2"); });
  server.begin();
  Serial.println("HTTP server started");
}
void webServerTask(void *pvParameters) {
    initWebServer();
    while (true) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void webSocketTask(void *pvParameters) {
    while (true) {
        ws.cleanupClients();
        ElegantOTA.loop();

        unsigned long now = millis();

        for (int i = 0; i < 5; ++i) {
            Sensor& s = sensors[i];
            if (now - s.lastSentTime >= s.period * 1000) {
                sendSensor(s);
                s.lastSentTime = now;
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS); // kiểm tra mỗi 100ms
    }
}

float readSensorValue(int id) {
    switch (id) {
        case 1: return getTempeDHT20();
        case 2: return getHumDHT20();
        case 3: return getLux();
        case 4: return getValueSMS();
        case 5: return getDistanceHC_SR04();
        default: return 0.0;
    }
}

void sendSensor(Sensor& s) {
    s.value = readSensorValue(s.id);

    JsonDocument doc;
    doc["type"] = "sensor";
    doc["id"] = s.id;
    doc["value"] = s.value;

    String jsonData;
    serializeJson(doc, jsonData);

    ws.textAll(jsonData);
}



void InitWebServer() {
    xTaskCreate(webServerTask, "WebServerTask", 16384, NULL, 1, NULL);
    xTaskCreate(webSocketTask, "WebSocketTask", 8192, NULL, 1, NULL);
}