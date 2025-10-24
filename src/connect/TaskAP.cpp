#include "include/TaskAP.h"

constexpr char AP_SSID[] = "ESP32-AP";
constexpr char AP_PASSWORD[] = "123456789";

void InitAPTask(void *pvParameters) {
    Serial.print("Setting AP (Access Point)…");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASSWORD);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Delete the task after initialization
    vTaskDelete(NULL);
}



void InitAP() {
    xTaskCreate(InitAPTask, "InitAPTask", 4096, NULL, 1, NULL);
}