/* telecommandes.c
 *
 * Utilisation de la télécommande (module Xbee vu comme une UART).
 * La télécommande envoie toutes les 100 ms un octet signé compris
 * entre -100 (babord toute) et +100 (tribord toute).
 *
 * Ce module fournit :
 *   - Telecommande_Init() : init de l’UART2 + IT RX
 *   - Telecommande_GetCommandeBrute() : dernière valeur reçue (-100..+100)
 *   - Telecommande_GetDirectionVitesse() :
 *         sens    = 0 -> gauche / babord
 *                 = 1 -> droite / tribord
 *         vitesse = 0..100 (%)
 */

#include "stm32f10x.h"
#include "Telecommande.h"
#include "../Driver1/USART.h"

/* Variable mise à jour dans USART.c, dans USART2_IRQHandler() */
extern int ch;   /* dernier octet reçu sur USART2 (-128..+127) */

/* Dernière commande mémorisée (copie locale en signé 8 bits) */
static int8_t g_last_cmd = 0;

/* ------------------------------------------------------------------------ */
/* Initialisation de la liaison avec la télécommande                        */
/* ------------------------------------------------------------------------ */

void Telecommande_Init(void)
{
    /* Configure GPIO + horloges + USART2 en 9600 bauds + RXNEIE
       (cf. USART2_Init dans USART.c). :contentReference[oaicite:0]{index=0} */
    USART2_Init();

    /* Active l’interruption USART2 dans le NVIC
       via le driver générique MyUSART_ActiveIT. :contentReference[oaicite:1]{index=1} */
    MyUSART_ActiveIT(USART2);

    /* Optionnel : si tu veux changer le débit, décommente la ligne suivante
       et modifie la valeur (en bauds) :
       MyUSART_SetBaudRate(USART2, 9600);
    */
}

/* ------------------------------------------------------------------------ */
/* Accès à la dernière valeur brute                                         */
/* ------------------------------------------------------------------------ */

/* Retourne la dernière valeur reçue sur l’UART, interprétée comme signé. */
int8_t Telecommande_GetCommandeBrute(void)
{
    g_last_cmd = (int8_t)ch;
    return g_last_cmd;
}

/* ------------------------------------------------------------------------ */
/* Conversion en (sens, vitesse)                                            */
/* ------------------------------------------------------------------------ */

/* Calcule sens/vitesse à partir de la dernière valeur reçue.
 *
 * sens    : 0 = babord (gauche), 1 = tribord (droite)
 * vitesse : 0..100 (%)
 */
void Telecommande_GetDirectionVitesse(char *sens, char *vitesse)
{
    int8_t cmd = (int8_t)ch;   /* -100 .. +100 envoyé par la télécommande */

    g_last_cmd = cmd;

    if (cmd > 0)
    {
        *sens    = 1;          /* tribord */
        *vitesse = (char)cmd;  /* 1..100 */
    }
    else if (cmd < 0)
    {
        *sens    = 0;          /* babord */
        *vitesse = (char)(-cmd); /* 1..100 */
    }
    else
    {
        /* cmd == 0 : stop */
        *sens    = 0;
        *vitesse = 0;
    }

    if ((unsigned char)(*vitesse) > 100)
    {
        *vitesse = 100;        /* sécurité si jamais la trame est hors plage */
    }
}
