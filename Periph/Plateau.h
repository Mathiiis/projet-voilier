/*
 * Pilotage du plateau rotatif (orientation du voilier).
 * Commande en vitesse de -100% (gauche) à +100% (droite).
 */
#ifndef __PLATEAU_H
#define __PLATEAU_H

#include "stm32f10x.h"
#include <stdint.h>

void InitPlateau(void);

void SetPlateau(float DC);

void USART3_Init_RX_Interrupt(void);

uint8_t USART3_Receive(void);

void SetPlateauSens(int sens);
void SetPlateauCommande(int8_t commande);

void USART3_SendString(char *s);

void USART3_SendChar(char c);


#endif
