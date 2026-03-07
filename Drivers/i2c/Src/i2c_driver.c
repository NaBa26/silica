#include "i2c_driver.h"
#include "gpio_driver.h"


static volatile I2C_State i2c_state = I2C_STATE_IDLE;

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

	I2C1->CR2 |= pclk;
	I2C->CR1 |= I2C_CR1_PE;


	NVIC_EnableIRQ(I2C1_EV_IRQn);
	NVIC_EnableIRQ(I2C1_ER_IRQn);

}

void I2C_write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t reg, uint8_t data){
	uint32_t sr1 = I2Cx->SR1;

	if (sr1 & I2C_SR1_SB)
	{
	   I2C1->DR = i2c_addr;
	}
}

uint8_t I2C_read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t reg){

}

void I2C_EV_IRQn(void){
	I2C1->SR1 |= I2C_SR1_BTF;
}

void I2C_ER_IRQn(void){
	I2C1->SR1 |= I2C_SR1_BTF;
}
