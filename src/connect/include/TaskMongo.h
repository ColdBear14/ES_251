#ifndef TASKMONGO_H
#define TASKMONGO_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "globals.h" // Chứa các biến toàn cục nếu cần

// Khai báo hàm khởi tạo Task gửi MongoDB (gọi 1 lần ở setup)
void initMongoTask(); 

// Các hàm đẩy dữ liệu vào hàng đợi (Queue)
void sendDataActionHistory(const String& name, const String& action);
void sendDataSensorData(const String& id, const String& sensorName, const String& sensorValue, const String& period);
void sendDataSettings(const int& id, const String& name, const int& period);

#endif // TASKMONGO_H
