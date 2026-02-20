#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "stm32f401xe.h"
#include <stdint.h>

#define UART_RX_BUF_SIZE   128U
#define UART_TX_BUF_SIZE   128U

void UART_Init(uint32_t baudrate, uint32_t pclk);
void UART_SendChar(uint8_t data);
void UART_SendString(const char *str);
uint8_t UART_ReadChar(uint8_t *data);

#endif
