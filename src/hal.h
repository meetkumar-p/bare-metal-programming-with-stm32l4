#ifndef HAL_H_
#define HAL_H_

#include <stdint.h>
#include <stdbool.h>

// clang-format off
#define BIT(x)          (1UL << (x))
#define PIN(bank, num)  ((((bank) - 'A') << 8) | (num))
#define PINNO(pin)      (pin & 255)
#define PINBANK(pin)    (pin >> 8)
// clang-format on

/**
 * @brief General Purpose I/O registers.
 *
 */
typedef struct GPIO_Registers
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
} GPIO_Registers;

// clang-format off
#define GPIO(bank)      ((GPIO_Registers *) (0x48000000 + 0x400 * (bank)))
// clang-format on

/**
 * @brief Reset and Clock Control registers.
 *
 */
typedef struct RCC_Registers
{
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLLSAI1CFGR;
    volatile uint32_t PLLSAI2CFGR;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    volatile uint32_t RESERVED0;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t RESERVED1;
    volatile uint32_t APB1RSTR1;
    volatile uint32_t APB1RSTR2;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RESERVED2;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t RESERVED3;
    volatile uint32_t APB1ENR1;
    volatile uint32_t APB1ENR2;
    volatile uint32_t APB2ENR;
    volatile uint32_t RESERVED4;
    volatile uint32_t AHB1SMENR;
    volatile uint32_t AHB2SMENR;
    volatile uint32_t AHB3SMENR;
    volatile uint32_t RESERVED5;
    volatile uint32_t APB1SMENR1;
    volatile uint32_t APB1SMENR2;
    volatile uint32_t APB2SMENR;
    volatile uint32_t RESERVED6;
    volatile uint32_t CCIPR;
    volatile uint32_t RESERVED7;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t CRRCR;
    volatile uint32_t CCIPR2;
    volatile uint32_t RESERVED8;
    volatile uint32_t DLYCFGR;
} RCC_Registers;

// clang-format off
#define RCC             ((RCC_Registers *) 0x40021000)
// clang-format on

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
 * @param pin   GPIO pin.
 * @param mode  GPIO pin's mode.
 */
void gpio_set_mode(const uint16_t pin, const GPIO_Modes mode);

/**
 * @brief Write to GPIO pin.
 *
 * @param pin   GPIO pin.
 * @param val   GPIO pin value.
 */
void gpio_write(const uint16_t pin, const bool val);

/**
 * @brief Produce a busy wait use NOP assembly instruction.
 *
 * @param count Number of times the NOP assembly instruction should be run.
 */
void spin(volatile uint32_t count);

#endif  // HAL_H_
