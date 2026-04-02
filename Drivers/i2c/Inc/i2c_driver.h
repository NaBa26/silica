#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

#include "stm32f401xe.h"

#define BMP280_ADDR 0x76  // SDO is connected to the ground for my configuration
#define I2C1_GPIO_AF 4U
#define I2C1_APB1_FREQ_MHZ 16U

typedef enum { I2C_READY = 0, I2C_BUSY_TX, I2C_BUSY_RX } I2C_State_t;

void I2C_ClockEnable(I2C_TypeDef *I2Cx);
void I2C1_Init(uint32_t pclk);
uint8_t I2C_write(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t *data,
                  uint16_t len);
uint8_t I2C_read(I2C_TypeDef *I2Cx, uint8_t slave_addr, uint8_t *buf,
                 uint16_t len);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);

#endif
