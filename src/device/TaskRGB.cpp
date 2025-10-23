#include "include/TaskRGB.h"

#define NUM_PIXELS     4 

String RGBState = "off";

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

// Bảng màu với các giá trị RGB
const uint32_t colorTable[] = {
  NeoPixel.Color(255, 0, 0),    // Đỏ
  NeoPixel.Color(255, 165, 0),  // Cam
  NeoPixel.Color(255, 255, 0),  // Vàng
  NeoPixel.Color(0, 255, 0),    // Lục
  NeoPixel.Color(0, 0, 255),  // Lam
  NeoPixel.Color(75, 0, 130),    // Chàm
  NeoPixel.Color(128, 0, 128),  // Tím
  NeoPixel.Color(255, 255, 255),// Trắng
  NeoPixel.Color(0, 0, 0)       // Đen
};

void TaskRGB(void *pvParameters)
{
    // Initialize RGB LED pins
    NeoPixel.begin();
    NeoPixel.clear();
    NeoPixel.show();

    while (1)
    {
        if(output2State == "on") {
            setRGB();
        }
        vTaskDelay(200 / portTICK_PERIOD_MS);  // Delay 2 giây
    }
}

void setRGB()
{
    for(int i = 0; i < sizeof(colorTable) / sizeof(colorTable[0]); i++) {

        if( output2State == "off") {
            NeoPixel.clear();
            NeoPixel.show();
            return;
        }

        // Set all pixels to the current color
        for(int j = 0; j < NUM_PIXELS; j++) {
            NeoPixel.setPixelColor(j, colorTable[i]);
        }
        NeoPixel.show();
        vTaskDelay(2000 / portTICK_PERIOD_MS);  // Delay 2 giây
    }
}

void initRGB()
{
    xTaskCreate(
      TaskRGB,    // Function to implement the task
      "TaskRGB",  // Name of the task
      4096,       // Stack size in words
      NULL,        // Task input parameter
      1,           // Priority of the task
      NULL         // Task handle
  );
}