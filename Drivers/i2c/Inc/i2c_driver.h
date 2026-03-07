#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f401xe.h"
#include <stdint.h>

#define	BMP280_ADDR		0x76 //SDO is connected to the ground for my configuration

typedef enum {
    I2C_STATE_IDLE,
    I2C_STATE_ADDR,
    I2C_STATE_REG,
    I2C_STATE_TX,
    I2C_STATE_RX,
    I2C_STATE_STOP
} I2C_State;



void I2C_ClockEnable(I2C_TypeDef *I2Cx);
void I2C1_Init(uint32_t pclk);
void I2C_write(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t I2C_read(uint8_t addr, uint8_t reg);
void I2C_IRQHandler(void);

#endif
