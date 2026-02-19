#include "uart_driver.h"

static volatile uint8_t  rx_buf[UART2_RX_BUF_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;

static volatile uint8_t  tx_buf[UART2_TX_BUF_SIZE];
static volatile uint16_t tx_head = 0;
static volatile uint16_t tx_tail = 0;


void UART2_Init(uint32_t baudrate, uint32_t pclk)
{
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2)));
    GPIOA->MODER |=  ((2U << (2*2)) | (2U << (3*2)));

    GPIOA->OSPEEDR |= (3U << (2*2)) | (3U << (3*2));

    GPIOA->PUPDR &= ~(3U << (3*2));
    GPIOA->PUPDR |=  (1U << (3*2));

    GPIOA->AFR[0] &= ~((0xFU << (4*2)) | (0xFU << (4*3)));
    GPIOA->AFR[0] |=  ((7U << (4*2)) | (7U << (4*3)));

    USART2->CR1 &= ~USART_CR1_UE;

    USART2->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_OVER8);
    USART2->CR2 &= ~USART_CR2_STOP;

    uint32_t usartdiv = pclk / baudrate;
    uint32_t mantissa = usartdiv / 16U;
    uint32_t fraction = usartdiv % 16U;

    if (fraction >= 16U)
    {
        mantissa++;
        fraction = 0U;
    }

    USART2->BRR = (mantissa << 4) | (fraction & 0xFU);

    USART2->CR1 |= USART_CR1_RE | USART_CR1_TE;
    USART2->CR1 |= USART_CR1_RXNEIE;

    NVIC_SetPriority(USART2_IRQn, 1);
    NVIC_EnableIRQ(USART2_IRQn);

    USART2->CR1 |= USART_CR1_UE;
}


void UART2_SendChar(uint8_t data)
{
    uint16_t next = (tx_head + 1) % UART2_TX_BUF_SIZE;

    while (next == tx_tail);

    tx_buf[tx_head] = data;
    tx_head = next;

    USART2->CR1 |= USART_CR1_TXEIE;
}

void UART2_SendString(const char *str)
{
    while (*str)
    {
        UART2_SendChar((uint8_t)*str++);
    }
}


uint8_t UART2_ReadChar(uint8_t *data)
{
    if (rx_head == rx_tail)
        return 0;

    *data = rx_buf[rx_tail];
    rx_tail = (rx_tail + 1) % UART2_RX_BUF_SIZE;

    return 1;
}


void USART2_IRQHandler(void)
{
    uint32_t sr = USART2->SR;

    if (sr & USART_SR_RXNE)
    {
        uint8_t data = (uint8_t)USART2->DR;

        uint16_t next = (rx_head + 1) % UART2_RX_BUF_SIZE;

        if (next != rx_tail)
        {
            rx_buf[rx_head] = data;
            rx_head = next;
        }
    }

    if ((sr & USART_SR_TXE) && (USART2->CR1 & USART_CR1_TXEIE))
    {
        if (tx_head == tx_tail)
        {
            USART2->CR1 &= ~USART_CR1_TXEIE;
        }
        else
        {
            USART2->DR = tx_buf[tx_tail];
            tx_tail = (tx_tail + 1) % UART2_TX_BUF_SIZE;
        }
    }

    if (sr & (USART_SR_ORE | USART_SR_FE | USART_SR_NE))
    {
        volatile uint32_t tmp = USART2->DR;
        (void)tmp;
    }
}
