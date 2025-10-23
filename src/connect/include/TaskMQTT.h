#ifndef INC_TASKMQTT_H_
#define INC_TASKMQTT_H_

#include "globals.h"

extern String IO_USERNAME;
extern String IO_KEY;

extern int MQTT_STATE;

extern void InitMQTT();

extern void initMQTT();

extern void InitMQTTTask();
void publishData(String feed, String data);

#endif /* INC_TASKMQTT_H_ */