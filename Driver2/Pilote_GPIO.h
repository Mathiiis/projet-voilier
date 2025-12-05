#ifndef __piloteGPIO_h
#define __piloteGPIO_h
#include "stm32f10x.h"

#define OUTPUTPUSHPULL           0x08
#define OUTPUTALTERNATEPUSHPULL  0x0A
#define INPUTFLOATING           0x04
#define INPUTANALOG             0x00

void InitGPIO(GPIO_TypeDef *PORT, char BROCHE, char CONFIG);
char LireBroche(GPIO_TypeDef *PORT, char BROCHE);
void SetBroche(GPIO_TypeDef *PORT, char BROCHE);
void ResetBroche(GPIO_TypeDef *PORT, char BROCHE);

#endif