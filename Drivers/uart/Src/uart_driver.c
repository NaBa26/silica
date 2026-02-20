#include "uart_driver.h"
#include "gpio_driver.h"


static volatile uint8_t rx_buf[UART_RX_BUF_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t tx_head = 0;

static volatile uint8_t tx_buf[UART_TX_BUF_SIZE];
static volatile uint8_t rx_tail = 0;
static volatile uint8_t tx_tail = 0;

void UART_Init(uint32_t baudrate, uint32_t pclk){
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //enable gpio


	GPIO_SetMode(GPIOA, 2, GPIO_MODE_AF);
	GPIO_SetMode(GPIOA, 3, GPIO_MODE_AF);
}
