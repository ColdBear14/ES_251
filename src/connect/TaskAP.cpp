#include "include/TaskAP.h"

constexpr char AP_SSID[] = "ESP32-AP";
constexpr char AP_PASSWORD[] = "123456789";

WiFiServer APserver(80);

String header;

void InitAPTask(void *pvParameters) {
    Serial.print("Setting AP (Access Point)…");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    APserver.begin();

    // Delete the task after initialization
    vTaskDelete(NULL);
}

void APloopTask(void *pvParameters) {
    while (true) {
        WiFiClient client = APserver.available();
        if (client) {
            Serial.println("New client");
            String currentLine = "";
            while (client.connected()) {
                if (client.available()) {
                    char c = client.read();
                    Serial.write(c);
                    header += c;
                    if (c == '\n') {
                        if (currentLine.length() == 0) {
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-Type: text/html");
                            client.println("Connection: close");
                            client.println();

                            if (header.indexOf("PUT /wifi") != -1) {
                                String ssid = header.substring(header.indexOf("ssid=") + 5, header.indexOf("&", header.indexOf("ssid=")));
                                String password = header.substring(header.indexOf("password=") + 9, header.indexOf("\r\n", header.indexOf("password=")));

                                Serial.print("SSID: ");
                                Serial.println(ssid);
                                Serial.print("Password: ");
                                Serial.println(password);
                            }

                            client.println("<!DOCTYPE HTML>");
                            client.println("<html><body><h1>ESP32 AP</h1>");
                            client.println("<p>Welcome to the ESP32 Access Point!</p>");
                            client.println("</body></html>");
                            break;
                        } else {
                            currentLine = "";
                        }
                    } else if (c != '\r') {
                        currentLine += c;
                    }
                }
                vTaskDelay(500 / portTICK_PERIOD_MS);
            }
            client.stop();
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void InitAP() {
    xTaskCreate(InitAPTask, "InitAPTask", 4096, NULL, 1, NULL);
    xTaskCreate(APloopTask, "APloopTask", 4096, NULL, 1, NULL);
}