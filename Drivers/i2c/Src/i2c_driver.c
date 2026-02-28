#include "i2c_driver.h"


void I2C_ClockEnable(I2C_TypeDef *I2Cx)
{
	if      (I2Cx == I2C1) RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	else if (I2Cx == I2C2) RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	else if (I2Cx == I2C3) RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
}

void I2C1_Init(void)
{
	I2C_ClockEnable(I2C1);

	GPIO_ClockEnable(GPIOB);
	GPIO_SetMode(GPIOB, 6, GPIO_MODE_AF);
	GPIO_SetMode(GPIOB, 7, GPIO_MODE_AF);
	GPIO_SetSpeed(GPIOA, 6, GPIO_SPEED_HIGH);
	GPIO_SetSpeed(GPIOA, 7, GPIO_SPEED_HIGH);
	GPIO_SetAlternateFunction(GPIOA, 2, I2C1_GPIO_AF);
	GPIO_SetAlternateFunction(GPIOA, 3, I2C1_GPIO_AF);

}
