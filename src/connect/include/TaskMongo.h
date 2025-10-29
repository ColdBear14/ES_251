#ifndef INC_TASKMONGO_H_
#define INC_TASKMONGO_H_

#include "globals.h"

void sendDataToMongoDBTask(void* parameter);
extern void sendDataActionHistory(const String& name, const String& action);
extern void sendDataSensorData(const String& sensorName, const String& sensorValue);
extern void sendDataSettings(const int& id, const String& name, const int& period);


#endif /* INC_TASKMONGO_H_ */