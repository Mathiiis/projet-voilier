#include "stm32f10x.h"
#include "Pilote_GPIO.h"

void InitGPIO(GPIO_TypeDef* PORT, char BROCHE, char CONFIG) {
	
	if (BROCHE <8) {
		PORT->CRL &= ~(0xF <<(BROCHE*4)) ;
		PORT->CRL |= (CONFIG <<(BROCHE*4)) ;
	} else {
		PORT->CRH &= ~(0xF << ((BROCHE-8)*4)) ;
		PORT->CRH |= (CONFIG <<((BROCHE-8)*4)) ;
	}
}

char LireBroche(GPIO_TypeDef *PORT, char BROCHE) {
	return (PORT->IDR & (0x1 << BROCHE)) >> BROCHE;
}
void SetBroche(GPIO_TypeDef *PORT, char BROCHE) {
	PORT->BSRR = (0x1 << BROCHE) ;
}
	
void ResetBroche(GPIO_TypeDef *PORT, char BROCHE) {
	PORT->BRR = (0x1 << BROCHE) ;
}
