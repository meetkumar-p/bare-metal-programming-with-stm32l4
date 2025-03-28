/**
 * @brief Application's entry point.
 *
 */
extern int main(void);

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

/**
 * @brief Initial stack value at the end of SRAM region.
 *
 */
extern void _estack(void);

/**
 * @brief SysTick IRQ handler.
 *
 */
extern void SysTick_Handler(void);

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
