#ifndef __adc_h
#define __adc_h
#include "stm32f10x.h"


void MyADC1_Config_PA0(void);
unsigned short MyADC1_Read_PA0_SWSTART(void);

float MyADC_GetConversion() ;

void MyADC_StartConvert();

void ADC1_Init_PA0_Interrupt(void);

#endif