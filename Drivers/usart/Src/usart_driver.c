	#include "usart_driver.h"
	#include "gpio_driver.h"


	static volatile uint8_t rx_buf[USART_RX_BUF_SIZE];
	static volatile uint8_t rx_head = 0;
	static volatile uint8_t tx_head = 0;

	static volatile uint8_t tx_buf[USART_TX_BUF_SIZE];
	static volatile uint8_t rx_tail = 0;
	static volatile uint8_t tx_tail = 0;

	void USART_ClockEnable(USART_TypeDef *USARTx)
	{
		if      (USARTx == USART1) RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		else if (USARTx == USART2) RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		else if (USARTx == USART6) RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	}

	void USART_Init(uint32_t baudrate, uint32_t pclk){
		USART_ClockEnable(USART2);
		GPIO_ClockEnable(GPIOA);


		GPIO_SetMode(GPIOA, 2, GPIO_MODE_AF);
		GPIO_SetMode(GPIOA, 3, GPIO_MODE_AF);
		GPIO_SetSpeed(GPIOA, 2, GPIO_SPEED_HIGH);
		GPIO_SetSpeed(GPIOA, 3, GPIO_SPEED_HIGH);
		GPIO_SetPull(GPIOA, 3, GPIO_PULL_UP);
		GPIO_SetAlternateFunction(GPIOA, 2, USART2_AF);
		GPIO_SetAlternateFunction(GPIOA, 3, USART2_AF);





	//	enable USART
		USA
	}
