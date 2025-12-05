#ifndef __myADC_h
#define __myADC_h
#include "stm32f10x.h"

extern int n;

void MyADC_Init(ADC_TypeDef * ADC, int ADCPrescaler) ;

void MyADC_StartConvert() ;

float MyADC_GetConversion() ;

#endif