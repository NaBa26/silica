#include "gpio_driver.h"

void GPIO_ClockEnable(GPIO_TypeDef *pGPIOx)
{
    if      (pGPIOx == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (pGPIOx == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (pGPIOx == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if (pGPIOx == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if (pGPIOx == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
}


void GPIO_SetMode(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t mode)
{
    if (pin > 15U || mode > GPIO_MODE_ANALOG) return;

    uint32_t pos = pin * 2U;
    pGPIOx->MODER &= ~(0x3U << pos);
    pGPIOx->MODER |=  ((uint32_t)mode << pos);
}

void GPIO_SetOutputType(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t type)
{
    if (pin > 15U || type > GPIO_OTYPE_OD) return;

    pGPIOx->OTYPER &= ~(1U << pin);
    pGPIOx->OTYPER |=  ((uint32_t)type << pin);
}

void GPIO_SetSpeed(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t speed)
{
    if (pin > 15U || speed > GPIO_SPEED_HIGH) return;

    uint32_t pos = pin * 2U;
    pGPIOx->OSPEEDR &= ~(0x3U << pos);
    pGPIOx->OSPEEDR |=  ((uint32_t)speed << pos);
}

void GPIO_SetPull(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t pull)
{
    if (pin > 15U || pull > GPIO_PULL_DOWN) return;

    uint32_t pos = pin * 2U;
    pGPIOx->PUPDR &= ~(0x3U << pos);
    pGPIOx->PUPDR |=  ((uint32_t)pull << pos);
}

void GPIO_SetAlternateFunction(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t af)
{
    if (pin > 15U || af > 15U) return;

    uint32_t index = pin / 8U;
    uint32_t pos   = (pin % 8U) * 4U;

    pGPIOx->AFR[index] &= ~(0xFU << pos);
    pGPIOx->AFR[index] |=  ((uint32_t)af << pos);
}


void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t pin, uint8_t state)
{
    if (pin > 15U) return;

    if (state == GPIO_PIN_SET)
        pGPIOx->BSRR = (1U << pin);
    else
        pGPIOx->BSRR = (1U << (pin + 16U));
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t pin)
{
    if (pin > 15U) return 0U;
    return (uint8_t)((pGPIOx->IDR >> pin) & 0x1U);
}

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t pin)
{
    if (pin > 15U) return;

    if ((pGPIOx->ODR >> pin) & 0x1U)
        pGPIOx->BSRR = (1U << (pin + 16U));
    else
        pGPIOx->BSRR = (1U << pin);
}
