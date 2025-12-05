/*
 * Pilotage du plateau rotatif (orientation du voilier).
 * Commande en vitesse de -100% (gauche) à +100% (droite).
 */
#ifndef __PLATEAU_H
#define __PLATEAU_H

#include "stm32f10x.h"
#include <stdint.h>

void Plateau_Init(void);
void Plateau_SetSpeed(int8_t percent);
int8_t Plateau_GetSpeed(void);

#endif
