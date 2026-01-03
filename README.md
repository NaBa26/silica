
# Silica

Bare-metal GPIO and UART drivers for STM32F401RE Nucleo board, written without HAL/LL libraries.

## Features

### GPIO Driver
- Clock enable/disable
- Pin mode configuration (input/output/alternate/analog)
- Digital read/write
- Pin toggle

### UART Driver
- Configurable baud rate
- Character and string transmission
- Character reception (blocking)
- Uses USART2 (PA2=TX, PA3=RX) via ST-Link virtual COM port

## Hardware
- **Board:** STM32F401RE Nucleo
- **MCU:** STM32F401RET6 (Cortex-M4, 84 MHz, 512KB Flash, 96KB RAM)
- **Clock:** 16 MHz HSI (default)

## Project Structure
```
├── Drivers/
│   ├── gpio/
│   │   ├── Inc/gpio_driver.h
│   │   └── Src/gpio_driver.c
│   └── uart/
│       ├── Inc/uart_driver.h
│       └── Src/uart_driver.c
├── Inc/
├── Src/
│   └── main.c
└── Startup/
    └── startup_stm32f401retx.s
```

## Terminal Setup
- **Baud Rate:** 115200
- **Data Bits:** 8
- **Parity:** None
- **Stop Bits:** 1
- **Flow Control:** None
- **Software:** PuTTY

## Build
- **IDE:** STM32CubeIDE 1.19.0
- **Toolchain:** ARM GCC
- **CMSIS:** Used for register definitions only (no HAL)

## Key Learnings
- Direct register manipulation for GPIO (MODER, ODR, BSRR, IDR)
- UART baud rate calculation using integer arithmetic (avoided FPU dependency)
- Alternate function configuration (AFR registers)
- Importance of clock enabling before peripheral access
- Polling status flags (TXE, RXNE) for reliable communication

## Future Improvements
- UART interrupt-based reception
- DMA support
- SPI and I2C drivers
- Timer/PWM drivers
