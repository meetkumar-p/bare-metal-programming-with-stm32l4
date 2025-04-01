#include "hal.h"

// clang-format off
#define SYSTICK_FREQUENCY   (1000U)
// clang-format on

void SystemInit(void)
{
    // enable coprocessors for FPU
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));

    // configure for selecting Range1 boost mode
    RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk);
    RCC->CFGR |= (8 << RCC_CFGR_HPRE_Pos) & RCC_CFGR_HPRE_Msk;
    PWR->CR5 &= ~(PWR_CR5_R1MODE_Msk);

    // set flash latency, and enable prefetch and instruction cache
    FLASH->ACR |= FLASH_LATENCY | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN;

    // clear necessary PLLCFGR bits
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLR_Msk | RCC_PLLCFGR_PLLREN_Msk | RCC_PLLCFGR_PLLN_Msk |
                      RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLSRC_Msk);

    // set HSI16 as PLL entry clock source
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;

    // set PLL_R, PLL_N, and PLL_M respectively
    RCC->PLLCFGR |= (((PLL_R - 2) / 2) << RCC_PLLCFGR_PLLR_Pos) & RCC_PLLCFGR_PLLR_Msk;
    RCC->PLLCFGR |= (PLL_N << RCC_PLLCFGR_PLLN_Pos) & RCC_PLLCFGR_PLLN_Msk;
    RCC->PLLCFGR |= ((PLL_M - 1) << RCC_PLLCFGR_PLLM_Pos) & RCC_PLLCFGR_PLLM_Msk;

    // enable PLLCLK output
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

    // enable HSI and wait until it is ready
    RCC->CR |= RCC_CR_HSION;
    while(0 == (RCC->CR & RCC_CR_HSIRDY))
    {
        spin(1);
    }

    // enable PLL and wait until it is ready
    RCC->CR |= RCC_CR_PLLON;
    while(0 == (RCC->CR & RCC_CR_PLLRDY))
    {
        spin(1);
    }

    // clear necessary CFGR bits
    RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk | RCC_CFGR_PPRE2_Msk);

    // set AHB, APB1, and APB2 prescalers, respectively
    RCC->CFGR |= (AHB_HPRE << RCC_CFGR_HPRE_Pos) & RCC_CFGR_HPRE_Msk;
    RCC->CFGR |= (APB1_PPRE << RCC_CFGR_PPRE1_Pos) & RCC_CFGR_PPRE1_Msk;
    RCC->CFGR |= (APB2_PPRE << RCC_CFGR_PPRE2_Pos) & RCC_CFGR_PPRE2_Msk;

    // set PLL as system clock and wait until it starts being used
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while(RCC_CFGR_SWS_PLL != (RCC->CFGR & RCC_CFGR_SWS_Msk))
    {
        spin(1);
    }

    // enable SYSCFG
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // configure SysTick to trigger SysTick IRQ every 1ms
    SysTick_Config(SYS_FREQUENCY / SYSTICK_FREQUENCY);
}
