#include "include/TaskAP.h"

constexpr char AP_SSID[] = "ESP32-Hy";
constexpr char AP_PASSWORD[] = "123456789";


void InitAP() {

    Serial.println("Setting AP");

    WiFi.mode(WIFI_AP_STA);

    bool ok = WiFi.softAP("ESP32-Hy", "123456789");
    if(!ok) {
        Serial.println("softAP failed!");
        return;
    }

    Serial.println("AP Started");
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

}