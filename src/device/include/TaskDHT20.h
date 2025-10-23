#ifndef INC_TASKDHT20_H_
#define INC_TASKDHT20_H_

#include "globals.h"

extern DHT20 dht20;

extern void TaskDHT20(void *pvParameters);

extern float getTempeDHT20();
extern float getHumDHT20();
extern void getValueDHT20();

extern void initDHT20();

#endif /* INC_TASKDHT20_H_ */