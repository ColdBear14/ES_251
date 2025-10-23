#ifndef INC_TASKWIFI_H_
#define INC_TASKWIFI_H_

#include "globals.h"

extern int WIFI_STATE;
extern String wifi_ssid;
extern String wifi_password;

extern void InitWiFi();

extern void initWifi();

extern bool Wifi_reconnect();

#endif /* INC_TASKWIFI_H_ */