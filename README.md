# Silica

Minimal bare-metal peripheral drivers for the STM32F401RE Nucleo board, implemented using direct register access (CMSIS only, no HAL/LL).

This project focuses on understanding peripheral internals, register maps, clock domains, and low-level hardware behavior rather than relying on abstraction layers.

---

## Hardware Platform

- **Board:** STM32F401RE Nucleo
- **MCU:** STM32F401RET6 (ARM Cortex-M4 @ 84 MHz)
  - 512 KB Flash
  - 96 KB SRAM
- **IDE:** STM32CubeIDE 1.19.0
- **Toolchain:** ARM GCC
- **Framework:** CMSIS (register definitions only, no HAL/LL)

---

## GPIO Driver

### Capabilities
- Peripheral clock enable/disable (RCC AHB1)
- Pin mode configuration (Input, Output, Alternate Function, Analog)
- Output control via ODR and BSRR
- Digital read via IDR
- Pin toggle utility
- Internal Pull-up/Pull-down (PUPD) configuration
- Slew rate / Output speed control

---

## UART Driver (USART2)

Configured for communication via ST-Link Virtual COM Port:
- **TX:** PA2
- **RX:** PA3

### Features
- **Polling-based Transmission:** Reliable character and string transmission using `TXE` (Transmit data register empty) and `TC` (Transmission complete) flags.
- **Interrupt-driven Reception:** Low-latency data handling via the `RXNE` (Read data register not empty) interrupt.
- **Format Support:** Standard 8N1 configuration (8 data bits, no parity, 1 stop bit).

### Technical Focus Areas
- **Manual Baud Rate Generation:** Direct calculation and assignment of the `USART_BRR` register based on $f_{PCLK}$.
- **Clock Tree Awareness:** Ensuring APB1/APB2 bus speeds are correctly factored into timing.
- **ISR Management:** Efficiently clearing and managing status flags within the vector table to prevent hang-ups.

---

## I2C Driver (I2C1)

- **SCL:** PB6
- **SDA:** PB7

### Features
- **Interrupt-Based FSM:** Non-blocking state machine handling Start, Address, Data (TX/RX), and Stop phases.
- **Master Mode:** Support for standard mode communication.
- **Error Handling:** Dedicated `I2C1_ER_IRQHandler` to catch and clear Acknowledge Failure (AF), Bus Errors (BERR), and Overruns.
- **Combined Sequences:** Support for `write_then_read` operations commonly used for sensor register polling.

### Technical Focus Areas
- **Timing Calculations:** Manual calculation of `CCR` (Clock Control Register) and `TRISE` (Maximum Rise Time) for specific I2C frequency targets.
- **Physical Layer Constraints:** Managing Open-Drain configurations. Since physical pull-ups are often absent, the driver utilizes internal pull-ups combined with adjusted GPIO slew-rate (Speed: Low) to stabilize signal rise times.
- **Repeated Start Logic:** Implementing seamless transitions between write and read phases without releasing the bus to maintain master ownership.

---

## Technical General Focus Areas
- Manual baud rate and frequency calculations.
- Peripheral clock tree awareness.
- Strict initialization sequences: Clock Enable → Pin Config → Peripheral Config → Enable Peripheral.
- Direct Status flag polling vs. Interrupt-driven logic.
- Avoidance of undefined behavior during direct register access.

---

## Planned Extensions
- SPI master driver
- Timer/PWM driver
- CAN implementation
- ADC (Analog to Digital Converter) DMA-based sampling

---

## Learning Outcomes
- Practical understanding of ARM Cortex-M4 peripheral architecture.
- Hands-on experience with RCC (Reset and Clock Control) and clock gating.
- Register-level debugging using the SFR (Special Function Register) view in STM32CubeIDE.
- Developing deterministic, lightweight drivers without middleware overhead.

---