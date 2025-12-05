#ifndef __piloteTelecommande_h
#define __piloteTelecommande_h 
#include "stm32f10x.h"

void Telecommande_Init(void);
int8_t Telecommande_GetCommandeBrute(void);
void Telecommande_GetDirectionVitesse(char *sens, char *vitesse);

#endif