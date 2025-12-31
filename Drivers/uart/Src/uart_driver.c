#include "uart_driver.h"

void UART_Init(USART_TypeDef *pUARTx, uint32_t baudrate, uint32_t pclk){
	pUARTx->CR1 &= ~(1 << 13);

	pUARTx->CR1 &= ~(1 << 12);
	pUARTx->CR1 &= ~(1 << 10);
	pUARTx->CR2 &= ~(3 << 12);

	uint32_t mantissa = pclk / (16 * baudrate);
	uint32_t fraction = ((pclk / baudrate) - (mantissa * 16));
	pUARTx->BRR = (mantissa << 4) | (fraction & 0xF);


	pUARTx->CR1 |= (1 << 3);
	pUARTx->CR1 |= (1 << 2);
	pUARTx->CR1 |= (1 << 13);
}

void UART_ClockEnable(USART_TypeDef *pUARTx){
	if(pUARTx == USART2){
	        RCC->APB1ENR |= (1 << 17);
	}
}

void UART_GPIO_Init(void){
	RCC->AHB1ENR |= (1 << 0);

	GPIOA->MODER &= ~(0xF << 4);
	GPIOA->MODER |= (0xA << 4);

	GPIOA->AFR[0] &= ~(0xFF << 8);
	GPIOA->AFR[0] |= (0x77 << 8);
}

void UART_SendChar(USART_TypeDef *pUARTx, uint8_t data){
	while(!(pUARTx->SR & (1<<7)));

	pUARTx->DR = data;
}

void UART_SendString(USART_TypeDef *pUARTx, char *str){
	for(int i=0;str[i]!='\0';i++){
		UART_SendChar(pUARTx, str[i]);
	}
}

uint8_t UART_ReceiveChar(USART_TypeDef *pUARTx){
	while(!(pUARTx->SR & (1<<5)));

	return (uint8_t)pUARTx->DR;
}
