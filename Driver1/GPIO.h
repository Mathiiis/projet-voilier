#ifndef __piloteGPIO_h
#define __piloteGPIO_h

#include "stm32f10x.h"

#define OUTPUTPUSHPULL           0x2   // CNF=00, MODE=10  0010b
#define OUTPUTALTERNATEPUSHPULL  0xA   // CNF=10, MODE=10  1010b
#define INPUTFLOATING           0x4   // CNF=01, MODE=00  0100b
#define INPUTANALOG             0x0   // CNF=00, MODE=00  0000b

void InitGPIO(GPIO_TypeDef *PORT, char BROCHE, char CONFIG);
char LireBroche(GPIO_TypeDef *PORT, char BROCHE);
void SetBroche(GPIO_TypeDef *PORT, char BROCHE);
void ResetBroche(GPIO_TypeDef *PORT, char BROCHE);
void MyGPIO_Toggle(GPIO_TypeDef *PORT, char BROCHE);

#endif

