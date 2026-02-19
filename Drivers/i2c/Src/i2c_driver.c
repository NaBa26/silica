#include "i2c_driver.h"


static volatile I2C_State_t i2c_state = I2C_READY;

static volatile uint8_t  *i2c_buf;
static volatile uint16_t  i2c_len;
static volatile uint16_t  i2c_index;
static volatile uint8_t   i2c_addr;


void I2C1_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    I2C1_GPIO_PORT->MODER &=
        ~((GPIO_MODER_MODER0 << (I2C1_SCL_PIN * 2U)) |
          (GPIO_MODER_MODER0 << (I2C1_SDA_PIN * 2U)));

    I2C1_GPIO_PORT->MODER |=
        ((GPIO_MODER_MODER0_1 << (I2C1_SCL_PIN * 2U)) |
         (GPIO_MODER_MODER0_1 << (I2C1_SDA_PIN * 2U)));

    I2C1_GPIO_PORT->OTYPER |=
        (GPIO_OTYPER_OT0 << I2C1_SCL_PIN) |
        (GPIO_OTYPER_OT0 << I2C1_SDA_PIN);

    I2C1_GPIO_PORT->OSPEEDR |=
        ((GPIO_OSPEEDER_OSPEEDR0 << (I2C1_SCL_PIN * 2U)) |
         (GPIO_OSPEEDER_OSPEEDR0 << (I2C1_SDA_PIN * 2U)));

    I2C1_GPIO_PORT->PUPDR &=
        ~((GPIO_PUPDR_PUPDR0 << (I2C1_SCL_PIN * 2U)) |
          (GPIO_PUPDR_PUPDR0 << (I2C1_SDA_PIN * 2U)));

    I2C1_GPIO_PORT->PUPDR |=
        ((GPIO_PUPDR_PUPDR0_0 << (I2C1_SCL_PIN * 2U)) |
         (GPIO_PUPDR_PUPDR0_0 << (I2C1_SDA_PIN * 2U)));

    uint32_t scl_index = I2C1_SCL_PIN / 8U;
    uint32_t scl_shift = (I2C1_SCL_PIN % 8U) * 4U;

    uint32_t sda_index = I2C1_SDA_PIN / 8U;
    uint32_t sda_shift = (I2C1_SDA_PIN % 8U) * 4U;

    I2C1_GPIO_PORT->AFR[scl_index] &=
        ~(0xFU << scl_shift);
    I2C1_GPIO_PORT->AFR[scl_index] |=
        (I2C1_GPIO_AF << scl_shift);

    I2C1_GPIO_PORT->AFR[sda_index] &=
        ~(0xFU << sda_shift);
    I2C1_GPIO_PORT->AFR[sda_index] |=
        (I2C1_GPIO_AF << sda_shift);


    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;

    I2C1->CR2 = I2C1_APB1_FREQ_MHZ;
    I2C1->CCR = I2C1_CCR_VALUE;
    I2C1->TRISE = I2C1_TRISE_VALUE;

    I2C1->CR1 |= I2C_CR1_ACK;

    NVIC_SetPriority(I2C1_EV_IRQn, 1);
    NVIC_EnableIRQ(I2C1_EV_IRQn);

    NVIC_SetPriority(I2C1_ER_IRQn, 1);
    NVIC_EnableIRQ(I2C1_ER_IRQn);

    I2C1->CR1 |= I2C_CR1_PE;
}

I2C_State_t I2C1_GetState(void)
{
    return i2c_state;
}

uint8_t I2C1_Master_Transmit_IT(uint8_t slave_addr,
                                uint8_t *data,
                                uint16_t len)
{
    if (i2c_state != I2C_READY || len == 0U)
        return 0U;

    i2c_state = I2C_BUSY_TX;
    i2c_buf   = data;
    i2c_len   = len;
    i2c_index = 0U;
    i2c_addr  = (slave_addr << 1U);

    I2C1->CR1 |= I2C_CR1_START;
    I2C1->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN;

    return 1U;
}

uint8_t I2C1_Master_Receive_IT(uint8_t slave_addr,
                               uint8_t *data,
                               uint16_t len)
{
    if (i2c_state != I2C_READY || len == 0U)
        return 0U;

    i2c_state = I2C_BUSY_RX;
    i2c_buf   = data;
    i2c_len   = len;
    i2c_index = 0U;
    i2c_addr  = (slave_addr << 1U) | 1U;

    I2C1->CR1 |= I2C_CR1_START;
    I2C1->CR2 |= I2C_CR2_ITEVTEN | I2C_CR2_ITBUFEN;

    return 1U;
}

void I2C1_EV_IRQHandler(void)
{
    uint32_t sr1 = I2C1->SR1;
    (void)I2C1->SR2;

    if (sr1 & I2C_SR1_SB)
    {
        I2C1->DR = i2c_addr;
    }
    else if (sr1 & I2C_SR1_ADDR)
    {
        if (i2c_state == I2C_BUSY_RX && i2c_len == 1U)
        {
            I2C1->CR1 &= ~I2C_CR1_ACK;
            I2C1->CR1 |= I2C_CR1_STOP;
        }
    }
    else if (sr1 & I2C_SR1_TXE)
    {
        if (i2c_state == I2C_BUSY_TX)
        {
            if (i2c_index < i2c_len)
            {
                I2C1->DR = i2c_buf[i2c_index++];
            }
            else
            {
                I2C1->CR1 |= I2C_CR1_STOP;
                i2c_state = I2C_READY;
            }
        }
    }
    else if (sr1 & I2C_SR1_RXNE)
    {
        if (i2c_state == I2C_BUSY_RX)
        {
            i2c_buf[i2c_index++] = I2C1->DR;

            if (i2c_index == (i2c_len - 1U))
            {
                I2C1->CR1 &= ~I2C_CR1_ACK;
                I2C1->CR1 |= I2C_CR1_STOP;
            }
            else if (i2c_index >= i2c_len)
            {
                i2c_state = I2C_READY;
                I2C1->CR1 |= I2C_CR1_ACK;
            }
        }
    }
}


void I2C1_ER_IRQHandler(void)
{
    I2C1->SR1 = 0U;
    I2C1->CR1 |= I2C_CR1_STOP;
    i2c_state = I2C_READY;
}
