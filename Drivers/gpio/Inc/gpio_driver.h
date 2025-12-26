#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f401xe.h"

#define GPIO_MODE_INPUT   0x00
#define GPIO_MODE_OUTPUT  0x01
#define GPIO_MODE_ALT     0x02
#define GPIO_MODE_ANALOG  0x03

void GPIO_PinMode(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t mode);
void GPIO_ClockEnable(GPIO_TypeDef *pGPIOx);
void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t value);
uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t pin);
void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t pin);

#endif
