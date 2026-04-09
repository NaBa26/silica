#include "gpio_driver.h"
#include "i2c_driver.h"

static volatile I2C_State_t i2c_state = I2C_READY;

static volatile uint8_t *i2c_buf;
static volatile uint16_t i2c_len;
static volatile uint16_t i2c_index;
static volatile uint8_t i2c_addr;

void I2C_ClockEnable(I2C_TypeDef *I2Cx) {
  if (I2Cx == I2C1)
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  else if (I2Cx == I2C2)
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
  else if (I2Cx == I2C3)
    RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
}

void I2C1_Init(uint32_t pclk) {
  I2C_ClockEnable(I2C1);

  GPIO_ClockEnable(GPIOB);
  GPIO_SetMode(GPIOB, 6, GPIO_MODE_AF);
  GPIO_SetMode(GPIOB, 7, GPIO_MODE_AF);
  GPIO_SetSpeed(GPIOB, 6, GPIO_SPEED_HIGH);
  GPIO_SetSpeed(GPIOB, 7, GPIO_SPEED_HIGH);
  GPIO_SetOutputType(GPIOB, 6, GPIO_OTYPE_OD);
  GPIO_SetOutputType(GPIOB, 7, GPIO_OTYPE_OD);
  GPIO_SetAlternateFunction(GPIOB, 6, I2C1_GPIO_AF);
  GPIO_SetAlternateFunction(GPIOB, 7, I2C1_GPIO_AF);

  I2C1->CR1 |= I2C_CR1_SWRST;
  I2C1->CR1 &= ~I2C_CR1_SWRST;

  I2C1->CR2 = pclk;
  I2C1->CCR = I2C1_CCR_VALUE;
  I2C1->TRISE = I2C1_TRISE_VALUE;

  I2C1->CR1 |= I2C_CR1_ACK;

  NVIC_EnableIRQ(I2C1_EV_IRQn);
  NVIC_EnableIRQ(I2C1_ER_IRQn);
  NVIC_SetPriority(I2C1_ER_IRQn, 1);
  NVIC_SetPriority(I2C1_EV_IRQn, 2);

  I2C1->CR2 |= I2C_CR2_ITERREN;

  // enable this at the end of init
  I2C1->CR1 |= I2C_CR1_PE;
}

I2C_State_t I2C1_GetState(void) { return i2c_state; }

uint8_t I2C_write(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t *data,
                  uint16_t len) {
  if (i2c_state != I2C_READY || len == 0U) return 0U;

  i2c_state = I2C_BUSY_TX;

  i2c_buf = data;
  i2c_len = len;
  i2c_index = 0U;
  i2c_addr = (slave_addr << 1U);

  I2C1->CR2 |= I2C_CR2_ITEVTEN;
  I2C1->CR2 |= I2C_CR2_ITBUFEN;

  I2C1->CR1 |= I2C_CR1_START;

  return 1U;
}

uint8_t I2C_read(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t *buf,
                 uint16_t len) {
  if (i2c_state != I2C_READY || len == 0U) return 0U;

  i2c_state = I2C_BUSY_RX;

  i2c_buf = buf;
  i2c_len = len;
  i2c_index = 0U;
  i2c_addr = (slave_addr << 1U) | 1U;

  I2C1->CR2 |= I2C_CR2_ITEVTEN;
  I2C1->CR2 |= I2C_CR2_ITBUFEN;

  I2C1->CR1 |= I2C_CR1_START;

  return 1U;
}

uint8_t I2C_write_then_read(I2C_TypeDef *I2Cx, uint8_t slave_addr,
                            uint8_t *data, uint16_t write_len, uint8_t *buf,
                            uint16_t read_len) {
  I2C_write(I2Cx, slave_addr, data, write_len);
  while (I2C1_GetState() != I2C_READY);
  I2C_read(I2Cx, slave_addr, buf, read_len);
  while (I2C1_GetState() != I2C_READY);
  return 1U;
}

void I2C1_EV_IRQHandler(void) {
  uint32_t sr1 = I2C1->SR1;
  // ISR would fire once the start bit is set
  if (sr1 & I2C_SR1_SB) {
    I2C1->DR = i2c_addr;
  }

  if (sr1 & I2C_SR1_ADDR) {
    if (i2c_state == I2C_BUSY_RX && i2c_len == 1U) {
      I2C1->CR1 &= ~I2C_CR1_ACK;
    }
    (void)I2C1->SR2;  // clear ADDR
    if (i2c_state == I2C_BUSY_RX && i2c_len == 1U) {
      I2C1->CR1 |= I2C_CR1_STOP;
    }
  }

  if (sr1 & I2C_SR1_TXE) {
    if (i2c_state == I2C_BUSY_TX) {
      if (i2c_index < i2c_len) {
        I2C1->DR = i2c_buf[i2c_index++];
      } else {
        I2C1->CR1 |= I2C_CR1_STOP;
        i2c_state = I2C_READY;
      }
    }
  }

  if (sr1 & I2C_SR1_RXNE) {
    if (i2c_state == I2C_BUSY_RX) {
      i2c_buf[i2c_index++] = I2C1->DR;

      if (i2c_index == i2c_len - 1U) {
        // just read second last byte
        I2C1->CR1 &= ~I2C_CR1_ACK;  // prepare NACK for last byte
        I2C1->CR1 |= I2C_CR1_STOP;
      } else if (i2c_index >= i2c_len) {
        i2c_state = I2C_READY;
        I2C1->CR1 |= I2C_CR1_ACK;
      }
    }
  }
}

void I2C1_ER_IRQHandler(void) {
  I2C1->SR1 = 0U;  // clear all error flags
  I2C1->CR1 |= I2C_CR1_STOP;
  i2c_state = I2C_READY;
}
