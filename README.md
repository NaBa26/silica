# Silica

Minimal bare-metal peripheral drivers for the STM32F401RE Nucleo board, implemented using direct register access (CMSIS only, no HAL/LL).

This project focuses on understanding peripheral internals, register maps, clock domains, and low-level hardware behavior rather than relying on abstraction layers.

---

## Hardware Platform

- **Board:** STM32F401RE Nucleo  
- **MCU:** STM32F401RET6 (ARM Cortex-M4 @ 84 MHz)  
  - 512 KB Flash  
  - 96 KB SRAM  
- **Clock Source:** 16 MHz HSI (default configuration)  
- **IDE:** STM32CubeIDE 1.19.0  
- **Toolchain:** ARM GCC  
- **Framework:** CMSIS (register definitions only, no HAL/LL)

---

## Architecture Overview

The drivers are structured in a modular format:
Drivers/
├── gpio/
│ ├── Inc/gpio_driver.h
│ └── Src/gpio_driver.c
└── uart/
├── Inc/uart_driver.h
└── Src/uart_driver.c


Design principles:

- Explicit clock enable before peripheral access  
- Bitfield-level register configuration  
- No dynamic memory usage  
- Polling-based control flow (deterministic behavior)  
- No dependency on HAL or vendor middleware  

---

## GPIO Driver

### Capabilities

- Peripheral clock enable/disable (RCC AHB1)
- Pin mode configuration  
  - Input  
  - Output  
  - Alternate Function  
  - Analog  
- Output control via ODR and BSRR  
- Digital read via IDR  
- Pin toggle utility  

---

## UART Driver (USART2)

Configured for communication via ST-Link Virtual COM Port:

- **TX:** PA2  
- **RX:** PA3  

### Features

## Technical Focus Areas

- Manual baud rate calculation  
- Peripheral clock tree awareness  
- Proper sequence: clock enable → configuration → enable peripheral  
- Status flag polling for reliable transmission  
- Avoidance of undefined behavior during peripheral access  

---

## I2C Driver (I2C1)


### Features

## Technical Focus Areas

---

## Planned Extensions
- Improving further complexity for I2C test code with slaves involved
- SPI master driver  
- Timer/PWM driver  
- CAN implementation  

---

## Learning Outcomes

- Practical understanding of ARM Cortex-M peripheral architecture  
- Hands-on experience with RCC and clock gating  
- Register-level debugging using SFR view  
- Developing deterministic drivers without middleware  

---


