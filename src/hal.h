#ifndef HAL_H_
#define HAL_H_

#include "stm32l4s5xx.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// clang-format off
#define BIT(x)                          (1UL << (x))
#define SETBITS(R, CLEARMASK, SETMASK)  (R) = (((R) & ~(CLEARMASK)) | (SETMASK))
#define PIN(bank, num)                  ((((bank) - 'A') << 8) | (num))
#define PINNO(pin)                      (pin & 255)
#define PINBANK(pin)                    (pin >> 8)

#define GPIO(bank)                      ((GPIO_TypeDef *) (0x48000000 + 0x400 * (bank)))

#define BYTE_MASK                       (0xFFU)

// 6.2: AHB clock <= 120MHz; APB1 clock <= 120MHz; APB2 clock <= 120MHz
// 3.3.3, Table 12: Configure flash latency (WS) according to clock frequency

// HSI clock frequency [MHz]
#define HSI_CLK                         (16U)

// run PLLCLK at 120MHz
#define PLL_INPUT                       (HSI_CLK)

// bit_field = PLL_M - 1
#define PLL_M                           (2U)

// bit_field = PLL_N = multiplication_factor
#define PLL_N                           (30U)

// bit_field = ((PLL_R - 2) / 2) & 3
#define PLL_R                           (2U)

// HCLK = SYSCLK
#define AHB_HPRE                        (0U)

// APB1 = AHB
#define APB1_PPRE                       (0U)

// APB2 = AHB
#define APB2_PPRE                       (0U)

// multiply by 1,000,000 as frequency is calculated in MHz
#define SYS_FREQUENCY                   ((PLL_INPUT * PLL_N / PLL_M / PLL_R) * 1000000)
#define FLASH_LATENCY                   (FLASH_ACR_LATENCY_5WS)

#if 0 == APB1_PPRE
    #define APB1_FREQUENCY              (SYS_FREQUENCY)
#else
    #define APB1_FREQUENCY              (SYS_FREQUENCY / BIT(APB1_PPRE - 3))
#endif

#if 0 == APB2_PPRE
    #define APB2_FREQUENCY              (SYS_FREQUENCY)
#else
    #define APB2_FREQUENCY              (SYS_FREQUENCY / BIT(APB2_PPRE - 3))
#endif
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
 * @brief General Purpose I/O pin output types.
 *
 */
typedef enum
{
    GPIO_OTYPE_PUSH_PULL,
    GPIO_OTYPE_OPEN_DRAIN
} GPIO_Output_Types;

/**
 * @brief General Purpose I/O pin output speeds.
 *
 */
typedef enum
{
    GPIO_OSPEED_LOW,
    GPIO_OSPEED_MEDIUM,
    GPIO_OSPEED_HIGH,
    GPIO_OSPEED_VERY_HIGH
} GPIO_Output_Speeds;

/**
 * @brief General Purpose I/O pin pull-up/pull-down configuration.
 *
 */
typedef enum
{
    GPIO_PUPDR_NONE,
    GPIO_PUPDR_UP,
    GPIO_PUPDR_DOWN
} GPIO_Pull_Type;

/**
 * @brief Initialize GPIO pin.
 *
 * @param pin           GPIO pin.
 * @param mode          Pin's mode.
 * @param output_type   Pin's output type.
 * @param output_speed  Pin's output speed.
 * @param pull_type     Pin's pull-up/pull-down configuration.
 * @param alt_func_num  Pin's alternate function number as per datasheet (Table 16 and 17).
 */
void gpio_init(const uint16_t pin, const GPIO_Modes mode, const GPIO_Output_Types output_type,
               const GPIO_Output_Speeds output_speed, const GPIO_Pull_Type pull_type,
               const uint8_t alt_func_num);

/**
 * @brief Initialize GPIO pin as an input pin.
 *
 * @param pin           GPIO pin.
 */
void gpio_input(const uint16_t pin);

/**
 * @brief Initialize GPIO pin as an output pin.
 *
 * @param pin           GPIO pin.
 */
void gpio_output(const uint16_t pin);

/**
 * @brief Read GPIO pin.
 *
 * @param pin           GPIO pin.
 *
 * @return `bool`       GPIO input state.
 */
bool gpio_read(const uint16_t pin);

/**
 * @brief Toggle GPIO pin output.
 *
 * @param pin           GPIO pin.
 */
void gpio_toggle(const uint16_t pin);

/**
 * @brief Write to GPIO pin.
 *
 * @param pin           GPIO pin.
 * @param val           GPIO pin state.
 */
void gpio_write(const uint16_t pin, const bool val);

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
