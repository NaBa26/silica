#include "gpio_driver.h"

void GPIO_PinMode(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t mode){
    if(pin > 15 || mode > 3) {
        return;
    }

    uint8_t bit_pos = pin * 2;
    pGPIOx->MODER &= ~(0x3 << bit_pos);
    pGPIOx->MODER |= (mode << bit_pos);
}

void GPIO_ClockEnable(GPIO_TypeDef *pGPIOx){
	if(pGPIOx == GPIOA){
		RCC->AHB1ENR |= (0x1 << 0);
	}

	else if(pGPIOx == GPIOB){
		RCC->AHB1ENR |= (0x1 << 1);
	}

	else if(pGPIOx == GPIOC){
		RCC->AHB1ENR |= (0x1 << 2);
	}

	else if(pGPIOx == GPIOD){
		RCC->AHB1ENR |= (0x1 << 3);
	}

	else if(pGPIOx == GPIOE){
		RCC->AHB1ENR |= (0x1 << 4);
	}

	else{
		return;
	}
}

void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t value){
	if(pin > 15) {
	        return;
	}

	if(value > 1){
			return;
	}

	uint8_t set_pos = pin;
	uint8_t reset_pos = (set_pos) + 0x10;

	if(value == 1){
		pGPIOx->BSRR = (0x1 << set_pos);
	}
	else{
		pGPIOx->BSRR = (0x1 << reset_pos);
	}
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t pin){
	if(pin > 15) {
		return 0xFF;
	}

	uint8_t bit_pos = pin;

	return ((pGPIOx->IDR & (0x1 << bit_pos)) >> bit_pos);
}

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t pin){
    if(pin > 15) {
        return;
    }

    pGPIOx->ODR ^= (1 << pin);
}
