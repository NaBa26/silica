#include "tim_driver.h"


void TIM_ClockEnable(TIM_TypeDef *pTIMx) {
  if (pTIMx == TIM1)
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
  else if (pTIMx == TIM2)
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  else if (pTIMx == TIM3)
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  else if (pTIMx == TIM4)
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  else if (pTIMx == TIM5)
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
  else if (pTIMx == TIM6)
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
  else if (pTIMx == TIM7)
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
  else if (pTIMx == TIM8)
    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
}

void TIM_Enable(TIM_TypeDef *pTIMx) {
  pTIMx->CR1 |= TIM_CR1_CEN;
}

void TIM_Disable(TIM_TypeDef *pTIMx) {
  pTIMx->CR1 &= ~TIM_CR1_CEN;
}

void TIM_SetMode(TIM_TypeDef *pTIMx, uint8_t mode){
	if (mode > TIM_CENTER_ALIGNED_MODE_3) return;

	pTIMx->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);
	pTIMx->CR1 |= (uint32_t)mode;
}

void TIM_Init(TIM_TypeDef *pTIMx, uint32_t prescaler, uint32_t arr){

}


