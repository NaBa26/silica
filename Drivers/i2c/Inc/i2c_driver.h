#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include "stm32f401xe.h"
#include <stdint.h>


#define I2C1_APB1_FREQ_MHZ        16U

#define I2C1_SPEED_HZ             100000U

#define I2C1_GPIO_PORT            GPIOB
#define I2C1_SCL_PIN              6U
#define I2C1_SDA_PIN              7U
#define I2C1_GPIO_AF              4U


#define I2C1_CCR_VALUE \
    ((I2C1_APB1_FREQ_MHZ * 1000000U) / (2U * I2C1_SPEED_HZ))

#define I2C1_TRISE_VALUE \
    (I2C1_APB1_FREQ_MHZ + 1U)


typedef enum
{
    I2C_READY = 0,
    I2C_BUSY_TX,
    I2C_BUSY_RX
} I2C_State_t;

void I2C1_Init(void);

uint8_t I2C1_Master_Transmit_IT(uint8_t slave_addr, uint8_t *data, uint16_t len);
uint8_t I2C1_Master_Receive_IT(uint8_t slave_addr, uint8_t *data, uint16_t len);
I2C_State_t I2C1_GetState(void);

#endif
