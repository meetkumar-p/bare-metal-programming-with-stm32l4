#include "hal.h"

void gpio_set_mode(const uint16_t pin, const GPIO_Modes mode)
{
    GPIO_Registers *gpio_bank = GPIO(PINBANK(pin));
    int pin_number = PINNO(pin);

    // clear existing pin mode
    gpio_bank->MODER &= ~(3U << (pin_number * 2));

    // set new pin mode
    gpio_bank->MODER |= (mode & 3U) << (pin_number * 2);
}

void gpio_write(const uint16_t pin, const bool val)
{
    GPIO_Registers *gpio_bank = GPIO(PINBANK(pin));
    int pin_number = PINNO(pin);

    // write pin state
    gpio_bank->BSRR = (1U << pin_number) << (val ? 0 : 16);
}

void systick_init(const uint32_t ticks)
{
    // SysTick timer is 24-bit
    if((ticks - 1) > 0x00FFFFFFU)
    {
        return;
    }

    SYSTICK->STRVR.RELOAD = (ticks - 1) & 0x00FFFFFFU;
    SYSTICK->STCVR.CURRENT = 0;

    // enable SysTick
    SYSTICK->STCSR.CLKSOURCE = 1;
    SYSTICK->STCSR.TICKINT = 1;
    SYSTICK->STCSR.ENABLE = 1;

    // enable SYSCFG
    RCC->APB2ENR |= BIT(0);
}

void spin(volatile uint32_t count)
{
    // execute NOP instruction a given number of times
    while(count--)
    {
        asm("nop");
    }
}
