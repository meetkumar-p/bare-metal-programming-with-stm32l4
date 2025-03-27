#ifndef HAL_H_
#define HAL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// clang-format off
#define SYSCLK                      (4000000U)
// clang-format on

// clang-format off
#define BIT(x)                      (1UL << (x))
#define PIN(bank, num)              ((((bank) - 'A') << 8) | (num))
#define PINNO(pin)                  (pin & 255)
#define PINBANK(pin)                (pin >> 8)
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
#define GPIO(bank)                  ((GPIO_Registers *) (0x48000000 + 0x400 * (bank)))
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
#define RCC                         ((RCC_Registers *) 0x40021000)

#define RCC_APB1ENR1_UART4EN_MASK   (1UL << 19U)
#define RCC_APB2ENR_SYSCFGEN_MASK   (1UL << 0U)
#define RCC_APB2ENR_USART1EN_MASK   (1UL << 14U)
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
#define SYSTICK                     ((SysTick_Registers *) 0xE000E010)
// clang-format on

/**
 * @brief Universal Synchronous/Asynchronous Receiver Transmitter Registers.
 *
 */
typedef struct USART_Registers
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
    volatile uint32_t PRESC;
} USART_Registers;

// clang-format off
#define USART1                      ((USART_Registers *) 0x40013800)
#define UART4                       ((USART_Registers *) 0x40004C00)

#define USART_CR1_UE                (1UL << 0U)
#define USART_CR1_RE                (1UL << 2U)
#define USART_CR1_TE                (1UL << 3U)

#define USART_ISR_RXNE_RXFNE_MASK   (1UL << 5U)
#define USART_ISR_TXE_TXFNF_MASK    (1UL << 7U)

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
void usart_init(USART_Registers *const usart, const uint32_t baud);

/**
 * @brief Returns whether USART receive data register is ready for reading.
 *
 * @param usart         USART peripheral.
 *
 * @return `bool`       Read ready state.
 */
static inline bool usart_read_ready(const USART_Registers *const usart)
{
    // if RXNE bit is set, read data is ready
    return usart->ISR & USART_ISR_RXNE_RXFNE_MASK;
}

/**
 * @brief Read a receive data byte from USART receive data register.
 *
 * @param usart         USART peripheral.
 *
 * @return `uint8_t`    Read data byte.
 */
static inline uint8_t usart_read_byte(const USART_Registers *const usart)
{
    return (uint8_t)(usart->RDR & BYTE_MASK);
}

/**
 * @brief Write a transmit data byte to USART transmit data register.
 *
 * @param usart         USART peripheral.
 * @param byte          Transmit data byte.
 */
static inline void usart_write_byte(USART_Registers *const usart, const uint8_t byte)
{
    usart->TDR = byte;
    while(0 == (usart->ISR & USART_ISR_TXE_TXFNF_MASK))
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
static inline void usart_write_buffer(USART_Registers *const usart, const char *buf, size_t len)
{
    while(0 < len--)
    {
        usart_write_byte(usart, *(uint8_t *)buf++);
    }
}

#endif  // HAL_H_
