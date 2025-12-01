#ifndef INC_TASKWIFI_H_
#define INC_TASKWIFI_H_

#include "globals.h"

extern bool WIFI_STATE;
extern bool WIFI_SEND;

extern String wifi_ssid;
extern String wifi_password;

extern void InitWifi();

extern bool Wifi_reconnect();

#endif /* INC_TASKWIFI_H_ */