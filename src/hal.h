#ifndef HAL_H_
#define HAL_H_

#include <stdint.h>
#include <stdbool.h>

// clang-format off
#define SYSCLK              (4000000U)
// clang-format on

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

// clang-format off
/**
 * @brief SysTick Control and Status Register.
 * 
 */
typedef struct SysTick_CS_Register
{
  uint32_t ENABLE                       : 1;
  uint32_t TICKINT                      : 1;
  uint32_t CLKSOURCE                    : 1;
  uint32_t                              : 13;
  uint32_t COUNTFLAG                    : 1;
  uint32_t                              : 15;
} SysTick_CS_Register;

/**
 * @brief SysTick Reload Value Register.
 * 
 */
typedef struct SysTick_RV_Register
{
  uint32_t RELOAD                       : 24;
  uint32_t                              : 8;
} SysTick_RV_Register;

/**
 * @brief SysTick Current Value Register.
 * 
 */
typedef struct SysTick_CV_Register
{
  uint32_t CURRENT                      : 32;
} SysTick_CV_Register;

/**
 * @brief SysTick Calibration Register.
 * 
 */
typedef struct SysTick_C_Register
{
  uint32_t TENMS                        : 24;
  uint32_t                              : 6;
  uint32_t SKEW                         : 1;
  uint32_t NOREF                        : 1;
} SysTick_C_Register;

// clang-format on

/**
 * @brief SysTick Registers.
 *
 */
typedef struct SysTick_Registers
{
    volatile SysTick_CS_Register STCSR;
    volatile SysTick_RV_Register STRVR;
    volatile SysTick_CV_Register STCVR;
    volatile SysTick_C_Register STCR;
} SysTick_Registers;

// clang-format off
#define SYSTICK         ((SysTick_Registers *) 0xE000E010)
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
 * @brief SysTick initialization.
 *
 * @param ticks Number of clock periods before SysTick IRQ is triggered.
 */
void systick_init(const uint32_t ticks);

/**
 * @brief Produce a busy wait use NOP assembly instruction.
 *
 * @param count Number of times the NOP assembly instruction should be run.
 */
void spin(volatile uint32_t count);

#endif  // HAL_H_
