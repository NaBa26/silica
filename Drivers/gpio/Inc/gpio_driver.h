#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f401xe.h"
#include <stdint.h>

#define GPIO_MODE_INPUT     0x0U
#define GPIO_MODE_OUTPUT    0x1U
#define GPIO_MODE_AF        0x2U
#define GPIO_MODE_ANALOG    0x3U

#define GPIO_OTYPE_PP       0x0U
#define GPIO_OTYPE_OD       0x1U

#define GPIO_SPEED_LOW      0x0U
#define GPIO_SPEED_MEDIUM   0x1U
#define GPIO_SPEED_FAST     0x2U
#define GPIO_SPEED_HIGH     0x3U

#define GPIO_NO_PULL        0x0U
#define GPIO_PULL_UP        0x1U
#define GPIO_PULL_DOWN      0x2U

#define GPIO_PIN_RESET      0x0U
#define GPIO_PIN_SET        0x1U

void GPIO_ClockEnable(GPIO_TypeDef *pGPIOx);

void GPIO_SetMode(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t mode);
void GPIO_SetOutputType(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t type);
void GPIO_SetSpeed(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t speed);
void GPIO_SetPull(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t pull);
void GPIO_SetAlternateFunction(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t af);

void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t state);
uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t pin);
void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t pin);

#endif
