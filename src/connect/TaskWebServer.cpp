#include "include/TaskWebServer.h"

// Khởi tạo máy chủ HTTP và WebSocket
AsyncWebServer server(8000); // Máy chủ HTTP chạy trên cổng 80
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
    else if (message.startsWith("{\"action\":\"get_history\"")) {
        // Xử lý yêu cầu lịch sử dữ liệu
        handleGetHistory(message);
    }
}

void handleGetHistory(const String &message) {
    JsonDocument doc;

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(doc, message);

    // Extract values from the JSON document
    int sensorID = doc["sensorId"].as<int>();

    switch (sensorID)
    {
    case 1: 
        getHistoryFromMongoDB("sensor", "1");
        break;
    case 2: 
        getHistoryFromMongoDB("sensor", "2");
        break;
    case 3: 
        getHistoryFromMongoDB("sensor", "3");
        break;
    default:
        break;
    }
}

void handleSettings(const String &message){
    JsonDocument doc;

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(doc, message);

    // Extract values from the JSON document
    int sensorID = doc["id"].as<int>();
    String name = doc["name"].as<String>();
    int period = doc["period"].as<int>();

    sendDataSettings(sensorID, String(name), period);

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

    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Extract values từ JSON và kiểm tra kỹ hơn
    const char* ssid = doc["ssid"];
    const char* password = doc["password"];

    // Kiểm tra SSID kỹ lưỡng
    if (ssid == nullptr || strlen(ssid) == 0) {
        Serial.println("Error: SSID is null or empty!");
        return;
    }

    if (strlen(ssid) > 32) { // SSID tối đa 32 ký tự
        Serial.println("Error: SSID too long! Max 32 characters.");
        return;
    }

    // Kiểm tra password
    if (password != nullptr && strlen(password) > 64) { // Password tối đa 64 ký tự
        Serial.println("Error: Password too long! Max 64 characters.");
        return;
    }

    // Gán giá trị sau khi đã kiểm tra
    wifi_ssid = String(ssid);
    wifi_password = (password != nullptr) ? String(password) : "";

    Serial.println("Received WiFi config:");
    Serial.println("SSID: " + wifi_ssid);
    Serial.println("Password length: " + String(wifi_password.length()));

    WIFI_SEND = 1;

    // Gọi hàm kết nối WiFi
    InitWifi();
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
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/home.html", "text/html"); });
    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/wifi.html", "text/html"); });
    server.on("/device", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });
    server.on("/dashboard", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/dashboard.html", "text/html"); });

    server.on("/images/logo.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(LittleFS, "/images/logo.png", "image/png"); });

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
        
        vTaskDelay(100 / portTICK_PERIOD_MS); // kiểm tra mỗi 100ms
    }
}

void sendWebSocketMessage(String message) {
    ws.textAll(message); 
}


void InitWebServer() {
    xTaskCreate(webServerTask, "WebServerTask", 20000, NULL, 1, NULL);
    xTaskCreate(webSocketTask, "WebSocketTask", 10000, NULL, 1, NULL);
}