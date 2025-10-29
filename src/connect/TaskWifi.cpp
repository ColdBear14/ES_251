#include "include/TaskWifi.h"

int WIFI_STATE = 0;

String wifi_ssid = "";
String wifi_password= "";

void InitWiFi()
{
    if(WIFI_STATE == 1){
        WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
        while (WiFi.status() != WL_CONNECTED)
        {
            vTaskDelay(500 / portTICK_PERIOD_MS);
            Serial.print(".");
        }
    Serial.println("Connected to WiFI");
    sendDataActionHistory("Wifi", "config successful");
    }   
}

bool Wifi_reconnect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        InitWiFi();
        return WiFi.status() == WL_CONNECTED;
    }
    return true;
}
