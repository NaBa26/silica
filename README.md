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

### Registers Used

- MODER  
- OTYPER  
- OSPEEDR  
- PUPDR  
- IDR  
- ODR  
- BSRR  
- AFRL / AFRH  

### Key Implementation Points

- Mode configuration uses bit masking instead of magic numbers  
- BSRR used for atomic set/reset operations  
- Alternate function mapping handled through AFR registers  

---

## UART Driver (USART2)

Configured for communication via ST-Link Virtual COM Port:

- **TX:** PA2  
- **RX:** PA3  

### Features

- Configurable baud rate (integer arithmetic, no FPU usage)  
- Character transmission  
- String transmission  
- Blocking character reception  
- Polling-based flag handling (TXE, RXNE)

### Registers Used

- CR1  
- CR2  
- BRR  
- SR  
- DR  

### Configuration

- Baud Rate: 115200  
- Data Bits: 8  
- Parity: None  
- Stop Bits: 1  
- Flow Control: None  

Terminal tested using PuTTY.

---

## Technical Focus Areas

- Manual baud rate calculation  
- Peripheral clock tree awareness  
- Proper sequence: clock enable → configuration → enable peripheral  
- Status flag polling for reliable transmission  
- Avoidance of undefined behavior during peripheral access  

---

## Limitations

- Polling-only implementation (no interrupt support yet)  
- No error handling for framing, overrun, or noise errors  
- No power management integration  
- No DMA support  

---

## Planned Extensions

- Interrupt-driven UART  
- I2C driver (interrupt-based state machine)  
- SPI master driver  
- Timer/PWM driver  
- DMA-based UART transmission  
- Basic ring buffer implementation  

---

## Learning Outcomes

- Practical understanding of ARM Cortex-M peripheral architecture  
- Hands-on experience with RCC and clock gating  
- Register-level debugging using SFR view  
- Developing deterministic drivers without middleware  

---

Silica is intended as a foundation for building a fully interrupt-driven, modular embedded driver stack from first principles.


