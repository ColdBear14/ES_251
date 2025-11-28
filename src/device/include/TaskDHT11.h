#ifndef INC_TASKDHT11_H_
#define INC_TASKDHT11_H_

#include "globals.h"


extern void TaskDHT11(void *pvParameters);

extern float getTempeDHT11();
extern float getHumDHT11();
extern void getValueDHT11();

extern void initDHT11();

#endif /* INC_TASKDHT20_H_ */