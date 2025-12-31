#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "stm32f401xe.h"

void UART_Init(USART_TypeDef *pUARTx, uint32_t baudrate, uint32_t pclk);
void UART_ClockEnable(USART_TypeDef *pUARTx);
void UART_GPIO_Init(void);
void UART_SendChar(USART_TypeDef *pUARTx, uint8_t data);
void UART_SendString(USART_TypeDef *pUARTx, char *str);
uint8_t UART_ReceiveChar(USART_TypeDef *pUARTx);


#endif
