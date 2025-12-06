#ifndef INC_TASKWEBSERVER_H_
#define INC_TASKWEBSERVER_H_

#include "globals.h"

extern String output1State;
extern String output2State;
extern String output3State;
extern String output4State;

struct Sensor;  // Forward declaration

extern void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
extern void parseWebSocketMessage(AsyncWebSocketClient *client, const String &message);

void sendSensor(Sensor &s);
void handleSettings(const String &message);
void handleWifiConfig(const String &message);
void handleMQTT(const String &message);
void sendWebSocketMessage(String message);

extern void InitWebServer();

#endif /* INC_TASKWEBSERVER_H_ */