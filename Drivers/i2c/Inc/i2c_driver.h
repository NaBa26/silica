#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f401xe.h"
#include <stdint.h>

void USART_ClockEnable(USART_TypeDef *USARTx);
void I2C1_Init(void);

#endif
