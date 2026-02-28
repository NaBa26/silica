#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "stm32f401xe.h"
#include <stdint.h>

#define USART_RX_BUF_SIZE   128U
#define USART_TX_BUF_SIZE   128U
#define USART2_GPIO_AF		 7U

void USART_ClockEnable(USART_TypeDef *USARTx);
void USART2_Init(uint32_t baudrate, uint32_t pclk);
void USART_SendChar(uint8_t data);
void USART_SendString(const char *str);
uint8_t USART_ReadChar(uint8_t *data);
void USART_IRQHandler(void);

#endif
