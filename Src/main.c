#include "gpio_driver.h"
#include "i2c_driver.h"
#include "stm32f401xe.h"
#include "usart_driver.h"

volatile uint32_t uwTick = 0;

static uint16_t dig_T1;
static int16_t dig_T2;
static int16_t dig_T3;
static uint16_t dig_P1;
static int16_t dig_P2;
static int16_t dig_P3;
static int16_t dig_P4;
static int16_t dig_P5;
static int16_t dig_P6;
static int16_t dig_P7;
static int16_t dig_P8;
static int16_t dig_P9;


void SysTick_Handler(void) { uwTick++; }

void delay_ms(uint32_t ms) {
  uint32_t start = uwTick;
  while ((uwTick - start) < ms);
}

void send_int(USART_TypeDef *USARTx, int32_t val){
    char buf[12];
    int i = 0;
    if(val < 0){ USART_SendChar(USARTx, '-'); val = -val; }
    if(val == 0){ USART_SendChar(USARTx, '0'); return; }
    while(val > 0){ buf[i++] = '0' + (val % 10); val /= 10; }
    while(i-- > 0) USART_SendChar(USARTx, buf[i+1]);
}

void blinky(void) {
  GPIO_ClockEnable(GPIOA);

  GPIO_SetMode(GPIOA, 5, GPIO_MODE_OUTPUT);
  GPIO_SetOutputType(GPIOA, 5, GPIO_OTYPE_PP);
  GPIO_SetSpeed(GPIOA, 5, GPIO_SPEED_LOW);
  GPIO_SetPull(GPIOA, 5, GPIO_NO_PULL);

  while (1) {
    GPIO_TogglePin(GPIOA, 5);
    delay_ms(500);
  }
}

void uart_sender(void) {
  /* LED setup */
  GPIO_ClockEnable(GPIOA);
  GPIO_SetMode(GPIOA, 5, GPIO_MODE_OUTPUT);
  GPIO_SetOutputType(GPIOA, 5, GPIO_OTYPE_PP);
  GPIO_SetSpeed(GPIOA, 5, GPIO_SPEED_LOW);
  GPIO_SetPull(GPIOA, 5, GPIO_NO_PULL);

  USART2_Init(115200U, SystemCoreClock);

  delay_ms(100);

  USART_SendString(USART2, "\r\n=== UART Test Menu ===\r\n");
  USART_SendString(USART2, "1. Toggle LED\r\n");
  USART_SendString(USART2, "2. Send message\r\n");
  USART_SendString(USART2, "3. Echo mode\r\n");
  USART_SendString(USART2, "Select: ");

  while (1) {
    uint8_t choice;

    if (USART_ReadChar(&choice)) {
      USART_SendChar(USART2, choice);
      USART_SendString(USART2, "\r\n");

      switch (choice) {
        case '1':
          GPIO_TogglePin(GPIOA, 5);
          USART_SendString(USART2, "LED toggled\r\n");
          break;

        case '2':
          USART_SendString(USART2, "Hello from STM32!\r\n");
          break;

        case '3':
          USART_SendString(USART2, "Echo mode - press ESC to exit\r\n");

          while (1) {
            uint8_t c;
            if (USART_ReadChar(&c)) {
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

int32_t t_fine;

int32_t bmp280_compensate_T_int32(int32_t adc_T) {
  int32_t var1, var2, T;
  var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
  var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
            ((adc_T >> 4) - ((int32_t)dig_T1))) >>
           12) *
          ((int32_t)dig_T3)) >>
         14;
  t_fine = var1 + var2;
  T = (t_fine * 5 + 128) >> 8;
  return T;
}

uint32_t bmp280_compensate_P_int32(uint32_t adc_P) {
  int32_t var1, var2;
  uint32_t p;
  var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
  var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)dig_P6);
  var2 = var2 + ((var1 * ((int32_t)dig_P5)) << 1);
  var2 = (var2 >> 2) + (((int32_t)dig_P4) << 16);
  var1 = (((dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) +
          ((((int32_t)dig_P2) * var1) >> 1)) >>
         18;
  var1 = ((((32768 + var1)) * ((int32_t)dig_P1)) >> 15);
  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p = (((uint32_t)(((int32_t)1048576) - adc_P) - (var2 >> 12))) * 3125;
  if (p < 0x80000000) {
    p = (p << 1) / ((uint32_t)var1);
  } else {
    p = (p / (uint32_t)var1) * 2;
  }
  var1 = (((int32_t)dig_P9) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
  var2 = (((int32_t)(p >> 2)) * ((int32_t)dig_P8)) >> 13;
  p = (uint32_t)((int32_t)p + ((var1 + var2 + dig_P7) >> 4));
  return p;
}

void sensor(void) {
  I2C1_Init(I2C1_APB1_FREQ_MHZ);
  USART2_Init(115200U, SystemCoreClock);

  uint8_t reg = 0x88;
  uint8_t calib[24];

  I2C_write_then_read(I2C1, BMP280_ADDR, &reg, 1, calib, 24);

  dig_T1 = (uint16_t)(calib[1] << 8 | calib[0]);
  dig_T2 = (int16_t)(calib[3] << 8 | calib[2]);
  dig_T3 = (int16_t)(calib[5] << 8 | calib[4]);
  dig_P1 = (uint16_t)(calib[7] << 8 | calib[6]);
  dig_P2 = (int16_t)(calib[9] << 8 | calib[8]);
  dig_P3 = (int16_t)(calib[11] << 8 | calib[10]);
  dig_P4 = (int16_t)(calib[13] << 8 | calib[12]);
  dig_P5 = (int16_t)(calib[15] << 8 | calib[14]);
  dig_P6 = (int16_t)(calib[17] << 8 | calib[16]);
  dig_P7 = (int16_t)(calib[19] << 8 | calib[18]);
  dig_P8 = (int16_t)(calib[21] << 8 | calib[20]);
  dig_P9 = (int16_t)(calib[23] << 8 | calib[22]);

  while (1) {
    reg = 0xF7;
    uint8_t raw[6];
    I2C_write_then_read(I2C1, BMP280_ADDR, &reg, 1, raw, 6);

    int32_t adc_T =
        ((uint32_t)raw[3] << 12) | ((uint32_t)raw[4] << 4) | (raw[5] >> 4);
    uint32_t adc_P =
        ((uint32_t)raw[0] << 12) | ((uint32_t)raw[1] << 4) | (raw[2] >> 4);

    int32_t temp = bmp280_compensate_T_int32(adc_T);
    uint32_t pr = bmp280_compensate_P_int32(adc_P);

    delay_ms(100);

    USART_SendString(USART2, "Temperature: \t");
    send_int(USART2, temp/100);
    USART_SendChar(USART2, '.');
    send_int(USART2, temp%100);
    USART_SendString(USART2, " C\r\n");
    USART_SendString(USART2, "----------------------------------------------\n");
    USART_SendString(USART2, "Pressure: \t");
    send_int(USART2, pr/100);
    USART_SendString(USART2, " Pa\r\n");
  }
}

int main(void) {
  SysTick_Config(SystemCoreClock / 1000U);

  sensor();
  //    uart_sender();
  //    blinky();
}
