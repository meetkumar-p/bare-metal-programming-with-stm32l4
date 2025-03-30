#ifndef HAL_H_
#define HAL_H_

#include "stm32l4s5xx.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// clang-format off
#define SYSCLK_FREQ                 (4000000U)
// clang-format on

// clang-format off
#define BIT(x)                      (1UL << (x))
#define PIN(bank, num)              ((((bank) - 'A') << 8) | (num))
#define PINNO(pin)                  (pin & 255)
#define PINBANK(pin)                (pin >> 8)

#define GPIO(bank)                  ((GPIO_TypeDef *) (0x48000000 + 0x400 * (bank)))

#define BYTE_MASK                   (0xFFU)
// clang-format on

/**
 * @brief Produce a busy wait using NOP assembly instruction.
 *
 * @param count     Number of times the NOP assembly instruction should be run.
 */
void spin(volatile uint32_t count);

/**
 * @brief General Purpose I/O pin modes. Enum values are per datasheet (section 8.4.1).
 *
 */
typedef enum
{
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
} GPIO_Modes;

/**
 * @brief Set GPIO pin's mode.
 *
 * @param pin           GPIO pin.
 * @param mode          GPIO pin's mode.
 */
void gpio_set_mode(const uint16_t pin, const GPIO_Modes mode);

/**
 * @brief Set GPIO pin's alternate function.
 *
 * @param pin           GPIO pin.
 * @param af_num        GPIO pin's alternate function number as per datasheet (Table 16 and 17).
 */
void gpio_set_af(const uint16_t pin, const uint8_t af_num);

/**
 * @brief Write to GPIO pin.
 *
 * @param pin           GPIO pin.
 * @param val           GPIO pin value.
 */
void gpio_write(const uint16_t pin, const bool val);

/**
 * @brief SysTick initialization.
 *
 * @param ticks         Number of clock periods before SysTick IRQ is triggered.
 */
void systick_init(const uint32_t ticks);

/**
 * @brief USART initialization.
 *
 * @param usart         USART peripheral.
 * @param baud          Baud rate.
 */
void usart_init(USART_TypeDef *const usart, const uint32_t baud);

/**
 * @brief Returns whether USART receive data register is ready for reading.
 *
 * @param usart         USART peripheral.
 *
 * @return `bool`       Read ready state.
 */
static inline bool usart_read_ready(const USART_TypeDef *const usart)
{
    // if RXNE bit is set, read data is ready
    return usart->ISR & USART_ISR_RXNE_RXFNE;
}

/**
 * @brief Read a receive data byte from USART receive data register.
 *
 * @param usart         USART peripheral.
 *
 * @return `uint8_t`    Read data byte.
 */
static inline uint8_t usart_read_byte(const USART_TypeDef *const usart)
{
    return (uint8_t)(usart->RDR & BYTE_MASK);
}

/**
 * @brief Write a transmit data byte to USART transmit data register.
 *
 * @param usart         USART peripheral.
 * @param byte          Transmit data byte.
 */
static inline void usart_write_byte(USART_TypeDef *const usart, const uint8_t byte)
{
    usart->TDR = byte;
    while(0 == (usart->ISR & USART_ISR_TXE_TXFNF))
    {
        // wait for transmission to complete
        spin(1);
    }
}

/**
 * @brief Write an array of transmit data bytes to USART transmit data register sequentially.
 *
 * @param usart         USART peripheral.
 * @param buf           Transmit data buffer.
 * @param len           Number of bytes to transmit.
 */
static inline void usart_write_buffer(USART_TypeDef *const usart, const char *buf, size_t len)
{
    while(0 < len--)
    {
        usart_write_byte(usart, *(uint8_t *)buf++);
    }
}

#endif  // HAL_H_
