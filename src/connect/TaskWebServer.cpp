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
    if (message.startsWith("{\"action\":\"device\"")) {
        handleDevice(message);
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



void handleDevice(const String &message){
    // Tìm vị trí của "device" và "state"
    int devicePos = message.indexOf("\"device\":");
    int statePos = message.indexOf("\"state\":");

    // Lấy giá trị device
    int commaPos = message.indexOf(",", devicePos);
    String deviceStr = message.substring(devicePos + 9, commaPos);
    deviceStr.trim();
    int device = deviceStr.toInt();

    // Lấy giá trị state
    int quote1 = message.indexOf("\"", statePos + 8);
    int quote2 = message.indexOf("\"", quote1 + 1);
    String state = message.substring(quote1 + 1, quote2);

    if(device == 1) {
        output1State = state;
    } else if(device == 2) {
        output2State = state;
    } else if (device == 3) {
        output3State = state;
    } else if (device == 4) {
        output4State = state;
    }
}

void handleMQTT(const String &message) {
    StaticJsonDocument<200> doc;

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
    StaticJsonDocument<200> doc;

    // Deserialize the JSON string
    DeserializationError error = deserializeJson(doc, message);

    // Extract values from the JSON document
    const char* ssid = doc["ssid"];
    const char* password = doc["password"];

    // Lưu SSID và mật khẩu vào biến toàn cục
    wifi_ssid = ssid;
    wifi_password = password;

    WIFI_STATE = 1;

    InitWiFi(); // Gọi hàm khởi tạo WiFi với SSID và mật khẩu mới
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

        if (ws.availableForWriteAll()) {
            // float temperature = getTemperature();
            // float humidity = getHumidity();
            // int lux = getLux();
            // float soil = getSoilMoisture();
            // int distance = getDistanceHC_SR04();

            // String jsonData = "{ \"temperature\": " + String(temperature) +
            //                   ", \"humidity\": " + String(humidity) +
            //                   ", \"lux\": " + String(lux) +
            //                   ", \"soil\": " + String(soil) +
            //                   ", \"distance\": " + String(distance) + "}";

            // ws.textAll(jsonData);

            // vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void InitWebServer() {
    xTaskCreate(webServerTask, "WebServerTask", 16384, NULL, 1, NULL);
    xTaskCreate(webSocketTask, "WebSocketTask", 8192, NULL, 1, NULL);
}