#include "gpio_driver.h"
#include "usart_driver.h"

static volatile uint8_t rx_buf[USART_RX_BUF_SIZE];
static volatile uint8_t rx_head = 0;
static volatile uint8_t tx_head = 0;

static volatile uint8_t tx_buf[USART_TX_BUF_SIZE];
static volatile uint8_t rx_tail = 0;
static volatile uint8_t tx_tail = 0;

void USART_ClockEnable(USART_TypeDef *USARTx) {
  if (USARTx == USART1)
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  else if (USARTx == USART2)
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  else if (USARTx == USART6)
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
}

void USART2_Init(uint32_t baudrate, uint32_t pclk) {
  USART_ClockEnable(USART2);

  GPIO_ClockEnable(GPIOA);
  GPIO_SetMode(GPIOA, 2, GPIO_MODE_AF);
  GPIO_SetMode(GPIOA, 3, GPIO_MODE_AF);
  GPIO_SetSpeed(GPIOA, 2, GPIO_SPEED_HIGH);
  GPIO_SetSpeed(GPIOA, 3, GPIO_SPEED_HIGH);
  GPIO_SetPull(GPIOA, 3, GPIO_PULL_UP);
  GPIO_SetAlternateFunction(GPIOA, 2, USART2_GPIO_AF);
  GPIO_SetAlternateFunction(GPIOA, 3, USART2_GPIO_AF);

  USART2->CR1 &= ~(
      USART_CR1_M | USART_CR1_PCE |
      USART_CR1_OVER8);  // no parity control, over 16 sampling and 8 bits data
  USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;

  uint32_t usartdiv = pclk / baudrate;
  uint32_t mantissa = usartdiv / 16U;
  uint32_t fraction = usartdiv % 16U;

  if (fraction >= 16U) {
    mantissa++;
    fraction = 0U;
  }

  USART2->BRR = (mantissa << 4) | (fraction & 0xFU);

  NVIC_EnableIRQ(USART2_IRQn);
  NVIC_SetPriority(USART2_IRQn, 1);
}

void USART_SendChar(USART_TypeDef *USARTx, uint8_t data) {
  uint8_t next = (tx_head + 1) % USART_TX_BUF_SIZE;

  while (next == tx_tail);

  tx_buf[tx_head] = data;
  tx_head = next;

  USARTx->CR1 |= USART_CR1_TXEIE;
}

void USART_SendString(USART_TypeDef *USARTx, const char *str) {
  while (*str) {
    USART_SendChar(USART2, (uint8_t)*str++);
  }
}

uint8_t USART_ReadChar(uint8_t *data) {
  if (rx_head == rx_tail) return 0;

  *data = rx_buf[rx_tail];
  rx_tail = (rx_tail + 1) % USART_RX_BUF_SIZE;

  return 1;
}

void USART2_IRQHandler(void) {
  uint32_t sr = USART2->SR;

  if (sr & USART_SR_RXNE) {
    uint8_t data = (uint8_t)USART2->DR;

    uint16_t next = (rx_head + 1) % USART_RX_BUF_SIZE;

    if (next != rx_tail) {
      rx_buf[rx_head] = data;
      rx_head = next;
    }
  }

  // even if I haven't enabled the transmitter interrupt, I still would add it
  // here as it's important for this condition
  if ((sr & USART_SR_TXE) & (USART2->CR1 & USART_CR1_TXEIE)) {
    if (tx_head == tx_tail) {
      USART2->CR1 &= ~(USART_CR1_TXEIE);
    } else {
      USART2->DR = tx_buf[tx_tail];
      tx_tail = (tx_tail + 1) % USART_TX_BUF_SIZE;
    }
  }

  // we would need to read DR to clear the error flags
  if (sr & (USART_SR_ORE | USART_SR_FE | USART_SR_NE)) {
    volatile uint32_t tmp = USART2->DR;
    (void)tmp;
  }
}
