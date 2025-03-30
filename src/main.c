#include "hal.h"

#include <stdint.h>
#include <stdio.h>

// clang-format off
#define SYSTICK_FREQUENCY   (1000U)

#define USART_DEBUG         (USART1)
#define USART_BAUD_RATE     (115200U)
// clang-format on

static volatile uint32_t systick_count;

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

    // set LEDs to output mode
    gpio_set_mode(led_user_1, GPIO_MODE_OUTPUT);
    gpio_set_mode(led_user_2, GPIO_MODE_OUTPUT);

    // initialize debug USART
    usart_init(USART_DEBUG, USART_BAUD_RATE);

    // declare timer and 500 ms period
    uint32_t timer = 0, period = 500;

    for(;;)
    {
        if(timer_expired(&timer, period, systick_count))
        {
            // toggle LED states and write them to debug USART
            static bool on = false;
            on = !on;
            gpio_write(led_user_1, on);
            gpio_write(led_user_2, !on);

            printf("LED 1: %d\tLED 2: %d\tsystick_count: %lu\r\n", on, !on,
                   (unsigned long)systick_count);
        }
    }

    return 0;
}

void SystemInit(void)
{
    // enable SYSCFG
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // configure SysTick to trigger SysTick IRQ every 1ms
    SysTick_Config(SYSCLK_FREQ / SYSTICK_FREQUENCY);
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
