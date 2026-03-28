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

    USART2_Init(115200U, 16000000U);

    delay_ms(100);

    USART_SendString(USART2, "\r\n=== UART Test Menu ===\r\n");
    USART_SendString(USART2, "1. Toggle LED\r\n");
    USART_SendString(USART2, "2. Send message\r\n");
    USART_SendString(USART2, "3. Echo mode\r\n");
    USART_SendString(USART2, "Select: ");

    while (1)
    {
        uint8_t choice;

        if (USART_ReadChar(&choice))
        {
        	USART_SendChar(USART2, choice);
            USART_SendString(USART2, "\r\n");

            switch (choice)
            {
                case '1':
                    GPIO_TogglePin(GPIOA, 5);
                    USART_SendString(USART2, "LED toggled\r\n");
                    break;

                case '2':
                	USART_SendString(USART2, "Hello from STM32!\r\n");
                    break;

                case '3':
                	USART_SendString(USART2, "Echo mode - press ESC to exit\r\n");

                    while (1)
                    {
                        uint8_t c;
                        if (USART_ReadChar(&c))
                        {
                            if (c == 0x1B)  // ESC
                                break;

                            USART_SendChar(USART2, c);
                        }
                    }

                    USART_SendString(USART2, "\r\nExited echo mode\r\n");
                    break;

                default:
                	USART_SendString(USART2, "Invalid option\r\n");
                    break;
            }

            USART_SendString(USART2, "\r\nSelect: ");
        }
    }
}

void pressure_sensor(void){
	I2C1_Init(pclk);
	I2C_write(I2Cx, BMP280_ADDR, data, 1);
	I2C_read(I2Cx, BMP280_ADDR, buf, 3);

}


int main(void)
{
    SysTick_Config(SystemCoreClock / 1000U);

    uart_sender();
//    blinky();
}
