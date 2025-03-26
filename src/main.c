#include "hal.h"

#include <stdint.h>

// clang-format off
#define SYSCLK              (4000000U)
#define SYSTICK_FREQUENCY   (1000U)
// clang-format on

static volatile uint32_t systick_count;

/**
 * @brief SysTick IRQ Handler.
 *
 */
void SysTick_Handler(void);

/**
 * @brief Precise millisecond clock for arbitrary periodic timers.
 *
 * @param t             Expiration time (in ticks).
 * @param prd           Period length (in ticks).
 * @param now           Current time (in ticks).
 *
 * @return `bool`       Timer expiration status.
 */
bool timer_expired(uint32_t *t, const uint32_t prd, const uint32_t now);

int main(void)
{
    uint16_t led_user_1 = PIN('A', 5);
    uint16_t led_user_2 = PIN('B', 14);

    // enable GPIO clocks for LEDs
    RCC->AHB2ENR |= BIT(PINBANK(led_user_1));
    RCC->AHB2ENR |= BIT(PINBANK(led_user_2));

    // set LEDs to output mode
    gpio_set_mode(led_user_1, GPIO_MODE_OUTPUT);
    gpio_set_mode(led_user_2, GPIO_MODE_OUTPUT);

    // initialize SysTick to trigger SysTick IRQ every 1ms
    systick_init(SYSCLK / SYSTICK_FREQUENCY);

    // declare timer and 500 ms period
    uint32_t timer = 0, period = 500;

    for(;;)
    {
        if(timer_expired(&timer, period, systick_count))
        {
            static bool on = true;
            gpio_write(led_user_1, on);
            on = !on;
            gpio_write(led_user_2, on);
        }
    }

    return 0;
}

void SysTick_Handler(void)
{
    systick_count++;
}

bool timer_expired(uint32_t *t, const uint32_t prd, const uint32_t now)
{
    // time wrapped? reset timer
    if((now + prd) < *t)
    {
        *t = 0;
    }

    // first poll? set expiration
    if(0 == *t)
    {
        *t = now + prd;
    }

    // not expired yet, return
    if(*t > now)
    {
        return false;
    }
    // next expiration time
    *t = (now - *t) > prd ? now + prd : *t + prd;

    // timer expired, return true
    return true;
}

/**
 * @brief Reset interrupt handler with the startup code.
 *
 */
__attribute__((naked, noreturn)) void _reset(void)
{
    // memset .bss to zero, and copy .data section to RAM region
    extern long _sbss, _ebss, _sdata, _edata, _sidata;

    for(long *dst = &_sbss; dst < &_ebss; dst++)  // cppcheck-suppress comparePointers
    {
        *dst = 0;
    }

    for(long *dst = &_sdata, *src = &_sidata; dst < &_edata;)
    {
        *dst++ = *src++;
    }

    main();

    for(;;) (void)0;
}

// defined in link.ld
extern void _estack(void);

// clang-format off
/**
 * @brief 16 standard and 95 STM32-specific handlers.
 *
 */
__attribute__((section(".vectors"))) void (*const tab[16 + 95])(void) =
{
    _estack,
    _reset,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    SysTick_Handler
};
// clang-format on
