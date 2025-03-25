#include "hal.h"

#include <stdint.h>

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

    for(;;)
    {
        gpio_write(led_user_1, true);
        gpio_write(led_user_2, false);
        spin(250000);
        gpio_write(led_user_1, false);
        gpio_write(led_user_2, true);
        spin(250000);
    }

    return 0;
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
    _reset
};
// clang-format on
