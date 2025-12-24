#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f401xe.h"
#include "core_cm4.h"


void GPIO_PinMode(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t mode);
void GPIO_ClockEnable(GPIO_TypeDef *pGPIOx);
void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t value);
uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t pin);
void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t pin);

#endif
