#ifndef UART2_DRIVER_H
#define UART2_DRIVER_H

#include "stm32f401xe.h"
#include <stdint.h>

#define UART2_RX_BUF_SIZE   128U
#define UART2_TX_BUF_SIZE   128U

void UART2_Init(uint32_t baudrate, uint32_t pclk);
void UART2_SendChar(uint8_t data);
void UART2_SendString(const char *str);
uint8_t UART2_ReadChar(uint8_t *data);

#endif
