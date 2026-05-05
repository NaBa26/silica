#ifndef TIM_DRIVER_H
#define TIM_DRIVER_H

#include <stdint.h>

#include "stm32f401xe.h"

#define TIM_EDGE_ALIGNED_MODE 0x0U
#define TIM_CENTER_ALIGNED_MODE_1 0x1U
#define TIM_CENTER_ALIGNED_MODE_2 0x2U
#define TIM_CENTER_ALIGNED_MODE_3 0x3U

void TIM_ClockEnable(TIM_TypeDef *pTIMx);
void TIM_Enable(TIM_TypeDef *pTIMx);
void TIM_Disable(TIM_TypeDef *pTIMx);
void TIM_Init(TIM_TypeDef *pTIMx, uint32_t prescaler, uint32_t arr);
void TIM_SetMode(TIM_TypeDef *pTIMx, uint8_t mode);

#endif
