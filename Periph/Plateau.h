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
#endif
