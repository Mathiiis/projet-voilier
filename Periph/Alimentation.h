#ifndef __Alimentation_h
#define __Alimentation_h
#include "stm32f10x.h"

void Alimentation_Init(void);
void Send_Warning(void);
float GetBatteryVoltage(void);

#endif