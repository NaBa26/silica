#include "i2c_driver.h"
#include "gpio_driver.h"


static volatile I2C_State_t i2c_state = I2C_READY;

static volatile uint8_t  *i2c_buf;
static volatile uint16_t  i2c_len;
static volatile uint16_t  i2c_index;
static volatile uint8_t   i2c_addr;

void I2C_ClockEnable(I2C_TypeDef *I2Cx)
{
	if      (I2Cx == I2C1) RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	else if (I2Cx == I2C2) RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	else if (I2Cx == I2C3) RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
}

void I2C1_Init(uint32_t pclk)
{
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

	I2C1->CR2 = I2C1_APB1_FREQ_MHZ;
	I2C1->CCR = I2C1_CCR_VALUE;
	I2C1->TRISE = I2C1_TRISE_VALUE;

	I2C1->CR1 |= I2C_CR1_ACK;

	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_EnableIRQ(I2C1_ER_IRQn);
	NVIC_SetPriority(I2C1_ER_IRQn, 1);
	NVIC_SetPriority(I2C1_EV_IRQn, 2);

	I2C1->CR2 |= I2C_CR2_ITEVTEN;
	I2C1->CR2 |= I2C_CR2_ITBUFEN;
	I2C1->CR2 |= I2C_CR2_ITERREN;

	// enable this at the end of init
	I2C1->CR1 |= I2C_CR1_PE;
}

I2C_State_t I2C1_GetState(void)
{
    return i2c_state;
}

uint8_t I2C_write(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t *data, uint16_t len){
	if (i2c_state != I2C_READY || len == 0U)
		        return 0U;

		i2c_state = I2C_BUSY_TX;

		i2c_buf   = data;
		i2c_len   = len;
		i2c_index = 0U;
		i2c_addr  = (slave_addr << 1U);

		I2C1->CR1 |= I2C_CR1_START;
		I2C1->CR2 |= I2C_CR2_ITEVTEN;
		I2C1->CR2 |= I2C_CR2_ITBUFEN;

		return 1U;
}

uint8_t I2C_read(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t *buf, uint16_t len){
	if (i2c_state != I2C_READY || len == 0U)
		        return 0U;

		i2c_state = I2C_BUSY_RX;
		i2c_buf   = data;
		i2c_len   = len;
		i2c_index = 0U;
		i2c_addr  = (slave_addr << 1U) | 1U;

		I2C1->DR |= I2C_DR_DR;

		I2C1->CR1 |= I2C_CR1_START;
		I2C1->CR2 |= I2C_CR2_ITEVTEN;
		I2C1->CR2 |= I2C_CR2_ITBUFEN;

		return 1U;
}

void I2C1_EV_IRQHandler(void){
	uint32_t sr1 = I2C1->SR1;
	//ISR would fire once the start bit is set
	if(sr1 & I2C_SR1_SB){
		I2C1->DR = i2c_addr;
	}

	if(sr1 & I2C_SR1_ADDR){
	    (void)I2C1->SR2;
	}

	if(sr1 & I2C_SR1_TXE){
	    if(i2c_index < i2c_len){
	        I2C1->DR = i2c_buf[i2c_index++];
	    } else {
	        I2C1->CR1 |= I2C_CR1_STOP;
	        i2c_state = I2C_READY;
	    }
	}


}

void I2C1_ER_IRQHandler(void){
	I2C1->SR1 |= I2C_SR1_BTF;
}
