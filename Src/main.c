#include "stm32f401xe.h"
#include "gpio_driver.h"
#include "i2c_driver.h"
#include "usart_driver.h"

volatile uint32_t uwTick = 0;


void SysTick_Handler(void)
{
    uwTick++;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = uwTick;
    while ((uwTick - start) < ms);
}


void blinky(void)
{
    GPIO_ClockEnable(GPIOA);

    GPIO_SetMode(GPIOA, 5, GPIO_MODE_OUTPUT);
    GPIO_SetOutputType(GPIOA, 5, GPIO_OTYPE_PP);
    GPIO_SetSpeed(GPIOA, 5, GPIO_SPEED_LOW);
    GPIO_SetPull(GPIOA, 5, GPIO_NO_PULL);

    while (1)
    {
        GPIO_TogglePin(GPIOA, 5);
        delay_ms(500);
    }
}


void uart_sender(void)
{
    /* LED setup */
    GPIO_ClockEnable(GPIOA);
    GPIO_SetMode(GPIOA, 5, GPIO_MODE_OUTPUT);
    GPIO_SetOutputType(GPIOA, 5, GPIO_OTYPE_PP);
    GPIO_SetSpeed(GPIOA, 5, GPIO_SPEED_LOW);
    GPIO_SetPull(GPIOA, 5, GPIO_NO_PULL);

    UART2_Init(115200U, 16000000U);

    delay_ms(100);

    UART2_SendString("\r\n=== UART Test Menu ===\r\n");
    UART2_SendString("1. Toggle LED\r\n");
    UART2_SendString("2. Send message\r\n");
    UART2_SendString("3. Echo mode\r\n");
    UART2_SendString("Select: ");

    while (1)
    {
        uint8_t choice;

        if (UART2_ReadChar(&choice))
        {
            UART2_SendChar(choice);
            UART2_SendString("\r\n");

            switch (choice)
            {
                case '1':
                    GPIO_TogglePin(GPIOA, 5);
                    UART2_SendString("LED toggled\r\n");
                    break;

                case '2':
                    UART2_SendString("Hello from STM32!\r\n");
                    break;

                case '3':
                    UART2_SendString("Echo mode - press ESC to exit\r\n");

                    while (1)
                    {
                        uint8_t c;
                        if (UART2_ReadChar(&c))
                        {
                            if (c == 0x1B)  // ESC
                                break;

                            UART2_SendChar(c);
                        }
                    }

                    UART2_SendString("\r\nExited echo mode\r\n");
                    break;

                default:
                    UART2_SendString("Invalid option\r\n");
                    break;
            }

            UART2_SendString("\r\nSelect: ");
        }
    }
}


void i2c_no_slave_test(void)
{
    I2C1_Init();

    static uint8_t dummy[1] = {0xAA};

    while (1)
    {
        if (I2C1_GetState() == I2C_READY)
        {
            I2C1_Master_Transmit_IT(0x50, dummy, 1);
        }

        if (I2C1->SR1 & I2C_SR1_AF)
        {
            I2C1->SR1 &= ~I2C_SR1_AF;
            I2C1->CR1 |= I2C_CR1_STOP;
        }
    }
}



int main(void)
{
    SysTick_Config(SystemCoreClock / 1000U);

    uart_sender();
//    blinky();
//    i2c_no_slave_test();
}
